#include "config.h"
#include "thread_pool.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/epoll.h>
#include <errno.h>

ServerConfig g_cfg;
LogContext *g_log = NULL;
ThreadPool *g_pool = NULL;

#define MAX_EVENTS 1024

// 连接信息，通过epoll_event.data.ptr传递
typedef struct {
    int fd;
    char ip[INET_ADDRSTRLEN];
    uint16_t port;
} ConnInfo;

void sig_exit(int sig)
{
    (void)sig;
    if (g_pool)
        pool_destroy(g_pool);
    if (g_log)
        log_destroy(g_log);
    printf("server graceful exit\n");
    exit(0);
}

int main(int argc, char **argv)
{
    signal(SIGINT, sig_exit);
    // 忽略SIGPIPE，防止写已关闭socket时进程被杀
    signal(SIGPIPE, SIG_IGN);

    // 加载配置
    load_config(&g_cfg, CONF_PATH);
    // 命令行端口优先级最高
    if (argc >= 2)
    {
        int tmp_port = atoi(argv[1]);
        if (tmp_port > 1024 && tmp_port < 65535)
            g_cfg.port = tmp_port;
    }
    // 初始化异步日志
    g_log = log_init(&g_cfg);
    if (!g_log)
        fprintf(stderr, "WARN: log init failed, running without logging\n");

    // 创建线程池
    g_pool = pool_create(g_cfg.thread_pool_num, g_cfg.task_queue_max);
    if (!g_pool)
    {
        perror("thread pool create failed");
        return 1;
    }

    // 网络初始化
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0)
    {
        perror("socket failed");
        return 1;
    }
    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(g_cfg.port);
    if (bind(listen_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("bind failed");
        return 1;
    }
    if (listen(listen_fd, g_cfg.max_listen_queue) < 0)
    {
        perror("listen failed");
        return 1;
    }
    printf("Server start, port:%d, thread:%d, queue_max:%d, root:%s\n",
           g_cfg.port, g_cfg.thread_pool_num, g_cfg.task_queue_max, g_cfg.root_path);

    // === 新增：epoll多路IO复用，替换原来的while(1) accept循环 ===
    int epfd = epoll_create1(0);
    if (epfd < 0)
    {
        perror("epoll_create1 failed");
        return 1;
    }

    // listen_fd也用ConnInfo包装，统一用epoll_data.ptr传递
    // listen_info是栈变量，main不退出前一直有效
    ConnInfo listen_info;
    listen_info.fd = listen_fd;
    listen_info.ip[0] = '-';
    listen_info.ip[1] = '\0';
    listen_info.port = 0;

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.ptr = &listen_info;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &ev) < 0)
    {
        perror("epoll_ctl add listen_fd failed");
        return 1;
    }

    struct epoll_event events[MAX_EVENTS];
    printf("epoll event loop started, max_events: %d\n", MAX_EVENTS);

    // epoll事件循环（替代原来的while(1) accept阻塞循环）
    while (1)
    {
        int nready = epoll_wait(epfd, events, MAX_EVENTS, -1); // -1 = 无限等待
        if (nready < 0)
        {
            if (errno == EINTR)
                continue; // 被信号中断，继续等待
            perror("epoll_wait failed");
            break;
        }

        for (int i = 0; i < nready; i++)
        {
            ConnInfo *info = events[i].data.ptr;

            if (info->fd == listen_fd)
            {
                // 新连接到达
                struct sockaddr_in client_addr;
                socklen_t cli_len = sizeof(client_addr);
                int cli_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &cli_len);
                if (cli_fd < 0)
                {
                    perror("accept failed");
                    continue;
                }
                char ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &client_addr.sin_addr, ip, INET_ADDRSTRLEN);
                uint16_t port = ntohs(client_addr.sin_port);

                // 为客户端连接创建ConnInfo
                ConnInfo *cli_info = malloc(sizeof(ConnInfo));
                if (!cli_info)
                {
                    close(cli_fd);
                    continue;
                }
                cli_info->fd = cli_fd;
                strncpy(cli_info->ip, ip, INET_ADDRSTRLEN - 1);
                cli_info->ip[INET_ADDRSTRLEN - 1] = '\0';
                cli_info->port = port;

                // 客户端fd加入epoll：EPOLLIN数据可读 + EPOLLRDHUP对端关闭
                struct epoll_event cev;
                cev.events = EPOLLIN | EPOLLRDHUP;
                cev.data.ptr = cli_info;
                if (epoll_ctl(epfd, EPOLL_CTL_ADD, cli_fd, &cev) < 0)
                {
                    perror("epoll_ctl add client_fd failed");
                    free(cli_info);
                    close(cli_fd);
                    continue;
                }
            }
            else
            {
                // 客户端数据就绪或对端关闭
                // 先从epoll移除，避免后续重复触发
                epoll_ctl(epfd, EPOLL_CTL_DEL, info->fd, NULL);

                if (events[i].events & EPOLLRDHUP)
                {
                    // 对端已关闭连接，直接清理不进线程池
                    close(info->fd);
                    free(info);
                }
                else
                {
                    // 数据可读，推入线程池处理
                    // pool_push_task内部会拷贝ip，这里释放cli_info安全
                    if (pool_push_task(g_pool, info->fd, info->ip, info->port) < 0)
                    {
                        // 队列满，拒绝连接
                        close(info->fd);
                    }
                    free(info);
                }
            }
        }
    }

    close(epfd);
    return 0;
}
