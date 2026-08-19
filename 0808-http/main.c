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

ServerConfig g_cfg;
LogContext *g_log = NULL;
ThreadPool *g_pool = NULL;

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
    // 关键：忽略SIGPIPE，防止写已关闭socket时进程被杀（压测必备）
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
    // 初始化异步日志（失败不致命，继续运行）
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

    // 循环accept（生产者）
    struct sockaddr_in client_addr;
    socklen_t cli_len = sizeof(client_addr);
    while (1)
    {
        int cli_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &cli_len);
        if (cli_fd < 0)
            continue;
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, ip, INET_ADDRSTRLEN);
        uint16_t port = ntohs(client_addr.sin_port);
        // 入队，队列满直接丢弃该连接
        if (pool_push_task(g_pool, cli_fd, ip, port) < 0)
        {
            close(cli_fd);
        }
    }
    return 0;
}
