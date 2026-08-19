#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include <pthread.h>
#include <stdint.h>
#include <arpa/inet.h>

// 任务：仅存放客户端fd与来源IP端口
typedef struct Task
{
    int fd;
    char ip[INET_ADDRSTRLEN];
    uint16_t port;
    struct Task *next;
} Task;

typedef struct ThreadPool
{
    pthread_mutex_t mtx;
    pthread_cond_t cond;
    int shutdown;
    int worker_num;
    int queue_capacity;
    int queue_size;
    Task *head;
    Task *tail;
} ThreadPool;

ThreadPool *pool_create(int worker_num, int queue_max);
int pool_push_task(ThreadPool *pool, int fd, const char *ip, uint16_t port);
void pool_destroy(ThreadPool *pool);
void *worker_routine(void *arg);
#endif