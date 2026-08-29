#include "thread_pool.h"
#include "log.h"
#include "http_util.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <bits/time.h>

// 注意：g_cfg 在 main.c 中是结构体全局变量(非指针)，故这里用 . 访问
extern ServerConfig g_cfg;
extern LogContext *g_log;

// 墙上时钟毫秒，用于统计请求耗时（clock()是CPU时间不准确）
static long now_ms(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long)ts.tv_sec * 1000L + ts.tv_nsec / 1000000L;
}

ThreadPool *pool_create(int worker_num, int queue_max)
{
    ThreadPool *pool = malloc(sizeof(ThreadPool));
    if (!pool)
        return NULL;
    pthread_mutex_init(&pool->mtx, NULL);
    pthread_cond_init(&pool->cond, NULL);
    pool->shutdown = 0;
    pool->worker_num = worker_num;
    pool->queue_capacity = queue_max;
    pool->queue_size = 0;
    pool->head = pool->tail = NULL;

    // 自定义线程栈：默认8MB太浪费，worker局部变量最大才~5KB
    // 256KB足够，12线程省约110MB虚拟地址空间
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, 256 * 1024);

    for (int i = 0; i < worker_num; i++)
    {
        pthread_t tid;
        pthread_create(&tid, &attr, worker_routine, pool);
        pthread_detach(tid);
    }
    pthread_attr_destroy(&attr);
    return pool;
}

int pool_push_task(ThreadPool *pool, int fd, const char *ip, uint16_t port)
{
    if (!pool || pool->shutdown)
        return -1;
    pthread_mutex_lock(&pool->mtx);
    if (pool->queue_size >= pool->queue_capacity)
    {
        pthread_mutex_unlock(&pool->mtx);
        return -2; // 队列满载，拒绝连接
    }
    Task *t = malloc(sizeof(Task));
    if (!t)
    {
        pthread_mutex_unlock(&pool->mtx);
        return -1;
    }
    t->fd = fd;
    strncpy(t->ip, ip, INET_ADDRSTRLEN - 1);
    t->ip[INET_ADDRSTRLEN - 1] = 0;
    t->port = port;
    t->next = NULL;
    if (!pool->head)
        pool->head = pool->tail = t;
    else
    {
        pool->tail->next = t;
        pool->tail = t;
    }
    pool->queue_size++;
    pthread_cond_signal(&pool->cond);
    pthread_mutex_unlock(&pool->mtx);
    return 0;
}

static Task *pool_pop(ThreadPool *pool)
{
    if (!pool->head)
        return NULL;
    Task *out = pool->head;
    pool->head = pool->head->next;
    if (!pool->head)
        pool->tail = NULL;
    pool->queue_size--;
    return out;
}

void pool_destroy(ThreadPool *pool)
{
    if (!pool)
        return;
    pthread_mutex_lock(&pool->mtx);
    pool->shutdown = 1;
    pthread_cond_broadcast(&pool->cond);
    pthread_mutex_unlock(&pool->mtx);
    sleep(1); // 等待worker退出
    Task *p = pool->head;
    while (p)
    {
        Task *tmp = p;
        p = p->next;
        free(tmp);
    }
    pthread_mutex_destroy(&pool->mtx);
    pthread_cond_destroy(&pool->cond);
    free(pool);
}

// worker消费者：处理单个HTTP连接
void *worker_routine(void *arg)
{
    ThreadPool *pool = (ThreadPool *)arg;
    char buf[HTTP_BUF_SIZE];
    // 套接字超时参数（线程内复用）
    struct timeval read_tv, write_tv;
    read_tv.tv_sec = g_cfg.read_timeout_ms / 1000;
    read_tv.tv_usec = (g_cfg.read_timeout_ms % 1000) * 1000;
    write_tv.tv_sec = g_cfg.write_timeout_ms / 1000;
    write_tv.tv_usec = (g_cfg.write_timeout_ms % 1000) * 1000;

    while (1)
    {
        Task *task = NULL;
        pthread_mutex_lock(&pool->mtx);
        while (pool->queue_size == 0 && !pool->shutdown)
            pthread_cond_wait(&pool->cond, &pool->mtx);
        if (pool->shutdown)
        {
            pthread_mutex_unlock(&pool->mtx);
            break;
        }
        task = pool_pop(pool);
        pthread_mutex_unlock(&pool->mtx);
        if (!task)
            continue;

        int fd = task->fd;
        char ip[INET_ADDRSTRLEN];
        uint16_t port = task->port;
        strncpy(ip, task->ip, INET_ADDRSTRLEN - 1);
        ip[INET_ADDRSTRLEN - 1] = 0;
        free(task);

        // 套接字参数优化
        setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &read_tv, sizeof(read_tv));
        setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &write_tv, sizeof(write_tv));
        int nodelay = 1;
        setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof(nodelay));

        long start = now_ms();
        memset(buf, 0, sizeof(buf));
        // 循环读完请求头，避免单次read只读到半个请求
        int n = read_http_request(fd, buf, HTTP_BUF_SIZE);
        if (n <= 0)
        {
            // 客户端未发数据或断开，静默关闭，不刷错误日志/不发响应
            close(fd);
            continue;
        }

        char uri[URI_MAX_LEN];
        int ret = parse_get_request(buf, uri);
        if (ret != 0)
        {
            log_append(g_log, LOG_WARN, ip, port, "-", 400, now_ms() - start);
            send_error_response(fd, 400);
            close(fd);
            continue;
        }
        // 先解码再做安全检测：解码后才可能出现真正的 ..
        url_decode(uri);
        if (path_safe_check(uri) < 0)
        {
            log_append(g_log, LOG_WARN, ip, port, uri, 403, now_ms() - start);
            send_error_response(fd, 403);
            close(fd);
            continue;
        }
        char real_path[FILEPATH_MAX];
        path_combine(real_path, FILEPATH_MAX, g_cfg.root_path, uri);
        int code = send_file_response(fd, real_path);
        long cost = now_ms() - start;
        if (code == 200)
            log_append(g_log, LOG_INFO, ip, port, uri, 200, cost);
        else
        {
            log_append(g_log, LOG_WARN, ip, port, uri, 404, cost);
            send_error_response(fd, 404);
        }
        close(fd);
    }
    return NULL;
}
