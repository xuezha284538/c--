#include "config.h"
#include <string.h>
#include <stdlib.h>

static void trim(char *s)
{
    char *p = s;
    while (*p == ' ' || *p == '\t')
        p++;
    char *e = p + strlen(p) - 1;
    while (e >= p && (*e == ' ' || *e == '\t' || *e == '\n' || *e == '\r'))
        e--;
    *(e + 1) = 0;
    memmove(s, p, strlen(p) + 1);
}

int load_config(ServerConfig *cfg, const char *conf_path)
{
    // 默认抗压兜底配置
    cfg->port = 8080;
    strcpy(cfg->root_path, "./html");
    cfg->max_listen_queue = 1024;
    cfg->thread_pool_num = 12;
    cfg->task_queue_max = 1024;
    cfg->log_enable = 1;
    strcpy(cfg->log_file, "server.log");
    cfg->log_flush_interval = 500;
    cfg->read_timeout_ms = 3000;
    cfg->write_timeout_ms = 3000;

    FILE *fp = fopen(conf_path, "r");
    if (!fp)
        return -1;
    char line[512];
    while (fgets(line, sizeof(line), fp))
    {
        trim(line);
        if (line[0] == '#' || line[0] == 0)
            continue;
        char *eq = strchr(line, '='); // 取第一个等号
        if (!eq)
            continue;
        *eq = 0;
        char k[256], v[256];
        strncpy(k, line, sizeof(k) - 1);
        k[sizeof(k) - 1] = 0;
        strncpy(v, eq + 1, sizeof(v) - 1);
        v[sizeof(v) - 1] = 0;
        trim(k);
        trim(v);
        if (!strcmp(k, "port"))
            cfg->port = atoi(v);
        else if (!strcmp(k, "root_path"))
        {
            strncpy(cfg->root_path, v, PATH_MAX_LEN - 1);
            cfg->root_path[PATH_MAX_LEN - 1] = 0;
        }
        else if (!strcmp(k, "max_listen_queue"))
            cfg->max_listen_queue = atoi(v);
        else if (!strcmp(k, "thread_pool_num"))
            cfg->thread_pool_num = atoi(v);
        else if (!strcmp(k, "task_queue_max"))
            cfg->task_queue_max = atoi(v);
        else if (!strcmp(k, "log_enable"))
            cfg->log_enable = atoi(v);
        else if (!strcmp(k, "log_file"))
        {
            strncpy(cfg->log_file, v, PATH_MAX_LEN - 1);
            cfg->log_file[PATH_MAX_LEN - 1] = 0;
        }
        else if (!strcmp(k, "log_flush_interval"))
            cfg->log_flush_interval = atoi(v);
        else if (!strcmp(k, "read_timeout_ms"))
            cfg->read_timeout_ms = atoi(v);
        else if (!strcmp(k, "write_timeout_ms"))
            cfg->write_timeout_ms = atoi(v);
    }
    fclose(fp);
    return 0;
}
