#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

LogContext *log_init(ServerConfig *cfg)
{
    LogContext *log = malloc(sizeof(LogContext));
    if (!log)
        return NULL;
    memset(log, 0, sizeof(*log));
    log->cfg = cfg;
    log->fp = fopen(cfg->log_file, "a+");
    if (!log->fp)
    {
        free(log);
        return NULL;
    }
    log->buf_len = 0;
    log->flush_interval = cfg->log_flush_interval;
    log->running = 1;
    pthread_mutex_init(&log->mtx, NULL);
    pthread_create(&log->log_tid, NULL, log_async_flush, log);
    pthread_detach(log->log_tid);
    return log;
}

void log_append(LogContext *log, LogLevel lv, const char *ip, uint16_t port, const char *uri, int code, long cost_ms)
{
    // log 可能为 NULL（日志初始化失败时），做防御
    if (!log || !log->cfg->log_enable || !log->running)
        return;
    pthread_mutex_lock(&log->mtx);
    time_t now = time(NULL);
    struct tm *tm = localtime(&now);
    char timestr[64];
    strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", tm);
    const char *lvstr;
    switch (lv)
    {
    case LOG_INFO:
        lvstr = "INFO";
        break;
    case LOG_WARN:
        lvstr = "WARN";
        break;
    case LOG_ERR:
        lvstr = "ERROR";
        break;
    default:
        lvstr = "INFO";
    }
    char tmp[512];
    int len = snprintf(tmp, sizeof(tmp),
                       "[%s][%s][%s:%u][URI:%s][CODE:%d][COST:%ldms]\n",
                       timestr, lvstr, ip, port, uri, code, cost_ms);
    if (len > 0 && log->buf_len + len < LOG_BUFFER_SIZE)
    {
        memcpy(log->buffer + log->buf_len, tmp, len);
        log->buf_len += len;
    }
    pthread_mutex_unlock(&log->mtx);
}

// 异步刷盘线程：定时把缓冲区写入文件
void *log_async_flush(void *arg)
{
    LogContext *log = (LogContext *)arg;
    while (log->running)
    {
        usleep(log->flush_interval * 1000);
        pthread_mutex_lock(&log->mtx);
        if (log->buf_len > 0)
        {
            fwrite(log->buffer, 1, log->buf_len, log->fp);
            fflush(log->fp);
            log->buf_len = 0;
        }
        pthread_mutex_unlock(&log->mtx);
    }
    return NULL;
}

void log_destroy(LogContext *log)
{
    if (!log)
        return;
    log->running = 0;
    sleep(1); // 等刷盘线程退出
    pthread_mutex_lock(&log->mtx);
    if (log->buf_len > 0)
    {
        fwrite(log->buffer, 1, log->buf_len, log->fp);
        fflush(log->fp);
    }
    pthread_mutex_unlock(&log->mtx);
    if (log->fp)
        fclose(log->fp);
    pthread_mutex_destroy(&log->mtx);
    free(log);
}
