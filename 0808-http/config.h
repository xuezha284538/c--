#ifndef CONFIG_H
#define CONFIG_H
#include <stdio.h>
#define CONF_PATH "server.conf"
#define PATH_MAX_LEN 256

typedef struct ServerConfig
{
    int port;
    char root_path[PATH_MAX_LEN];
    int max_listen_queue;

    int thread_pool_num;
    int task_queue_max;

    int log_enable;
    char log_file[PATH_MAX_LEN];
    int log_flush_interval;

    int read_timeout_ms;
    int write_timeout_ms;
} ServerConfig;

int load_config(ServerConfig *cfg, const char *conf_path);
#endif