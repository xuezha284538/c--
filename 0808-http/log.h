#ifndef LOG_H
#define LOG_H
#include "config.h"
#include <pthread.h>
#include <time.h>
#include <stdint.h>

typedef enum
{
    LOG_INFO,
    LOG_WARN,
    LOG_ERR
} LogLevel;

#define LOG_BUFFER_SIZE 4096
typedef struct LogContext
{
    FILE *fp;
    pthread_t log_tid;
    pthread_mutex_t mtx;
    char buffer[LOG_BUFFER_SIZE];
    int buf_len;
    int flush_interval;
    int running;
    ServerConfig *cfg;
} LogContext;

LogContext *log_init(ServerConfig *cfg);
void log_append(LogContext *log, LogLevel lv, const char *ip, uint16_t port, const char *uri, int code, long cost_ms);
void log_destroy(LogContext *log);
void *log_async_flush(void *arg);
#endif