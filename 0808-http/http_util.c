#include "http_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/sendfile.h>
#include <sys/stat.h>

// MIME类型查询：取最后一个'.'之后的扩展名判断
const char *get_mime_type(const char *path)
{
    const char *dot = strrchr(path, '.');
    if (!dot)
        return "application/octet-stream";
    if (!strcasecmp(dot, ".html") || !strcasecmp(dot, ".htm"))
        return "text/html; charset=utf-8";
    if (!strcasecmp(dot, ".css"))
        return "text/css";
    if (!strcasecmp(dot, ".js"))
        return "application/javascript";
    if (!strcasecmp(dot, ".jpg") || !strcasecmp(dot, ".jpeg"))
        return "image/jpeg";
    if (!strcasecmp(dot, ".png"))
        return "image/png";
    if (!strcasecmp(dot, ".gif"))
        return "image/gif";
    if (!strcasecmp(dot, ".ico"))
        return "image/x-icon";
    if (!strcasecmp(dot, ".bmp"))
        return "image/bmp";
    if (!strcasecmp(dot, ".mp4"))
        return "video/mp4";
    if (!strcasecmp(dot, ".mp3"))
        return "audio/mpeg";
    if (!strcasecmp(dot, ".m4a"))
        return "audio/mp4";
    if (!strcasecmp(dot, ".wav"))
        return "audio/wav";
    if (!strcasecmp(dot, ".pdf"))
        return "application/pdf";
    if (!strcasecmp(dot, ".txt"))
        return "text/plain; charset=utf-8";
    return "application/octet-stream";
}

// URL解码：原地解码 %XX 与 +，支持中文/空格文件名
void url_decode(char *s)
{
    char *src = s, *dst = s;
    while (*src)
    {
        if (*src == '%' && isxdigit((unsigned char)src[1]) && isxdigit((unsigned char)src[2]))
        {
            char hex[3] = {src[1], src[2], 0};
            *dst++ = (char)strtol(hex, NULL, 16);
            src += 3;
        }
        else if (*src == '+')
        {
            *dst++ = ' ';
            src++;
        }
        else
        {
            *dst++ = *src++;
        }
    }
    *dst = 0;
}

int parse_get_request(char *buf, char *uri_out)
{
    char method[32], uri[URI_MAX_LEN], proto[32];
    // 限定宽度，防止越界
    if (sscanf(buf, "%31s %255s %31s", method, uri, proto) != 3)
        return -1;
    if (strcmp(method, "GET") != 0)
        return -2;
    // 去掉查询串 ?xxx
    char *q = strchr(uri, '?');
    if (q)
        *q = 0;
    strncpy(uri_out, uri, URI_MAX_LEN - 1);
    uri_out[URI_MAX_LEN - 1] = 0;
    return 0;
}

int path_safe_check(const char *uri)
{
    // 防目录穿越：禁止任何 .. 出现
    if (strstr(uri, ".."))
        return -1;
    return 0;
}

void path_combine(char *dst, int dst_size, const char *root, const char *uri)
{
    if (uri[0] == '/')
        snprintf(dst, dst_size, "%s%s", root, uri);
    else
        snprintf(dst, dst_size, "%s/%s", root, uri);
    int len = strlen(dst);
    // 末尾是'/'说明访问目录，补 index.html
    if (len > 0 && dst[len - 1] == '/')
        snprintf(dst + len, dst_size - len, "index.html");
}

// 循环写，确保把 n 字节全部写完（处理部分写、EINTR）
ssize_t write_all(int fd, const void *buf, size_t n)
{
    size_t total = 0;
    const char *p = (const char *)buf;
    while (total < n)
    {
        ssize_t w = write(fd, p + total, n - total);
        if (w < 0)
        {
            if (errno == EINTR)
                continue;
            return -1; // EPIPE 等，客户端已断开
        }
        if (w == 0)
            break;
        total += w;
    }
    return (ssize_t)total;
}

void send_error_response(int fd, int code)
{
    const char *status;
    const char *body;
    switch (code)
    {
    case 400:
        status = "400 Bad Request";
        body = "400 Bad Request";
        break;
    case 403:
        status = "403 Forbidden";
        body = "403 Forbidden";
        break;
    case 404:
        status = "404 Not Found";
        body = "404 Not Found";
        break;
    default:
        status = "500 Internal Server Error";
        body = "500 Internal Server Error";
        break;
    }
    // Content-Length 由实际 body 长度计算，保证准确
    char buf[512];
    int len = snprintf(buf, sizeof(buf),
                       "HTTP/1.1 %s\r\n"
                       "Content-Type: text/plain; charset=utf-8\r\n"
                       "Content-Length: %zu\r\n"
                       "Connection: close\r\n\r\n%s",
                       status, strlen(body), body);
    write_all(fd, buf, (size_t)len);
}

int send_file_response(int fd, const char *filepath)
{
    struct stat st;
    if (stat(filepath, &st) < 0)
        return -1;
    if (!S_ISREG(st.st_mode))
        return -1; // 目录或其它非普通文件 → 404
    int f = open(filepath, O_RDONLY);
    if (f < 0)
        return -1;
    char header[512];
    const char *mime = get_mime_type(filepath);
    int hlen = snprintf(header, sizeof(header),
                        "HTTP/1.1 200 OK\r\n"
                        "Content-Type: %s\r\n"
                        "Content-Length: %lld\r\n"
                        "Connection: close\r\n\r\n",
                        mime, (long long)st.st_size);
    if (write_all(fd, header, (size_t)hlen) < 0)
    {
        close(f);
        return -1;
    }
    // sendfile 循环，处理部分发送、EINTR、EAGAIN
    off_t offset = 0;
    size_t remaining = (size_t)st.st_size;
    while (remaining > 0)
    {
        ssize_t sent = sendfile(fd, f, &offset, remaining);
        if (sent < 0)
        {
            if (errno == EINTR || errno == EAGAIN)
                continue;
            break; // EPIPE 等，客户端断开，停止发送
        }
        if (sent == 0)
            break;
        remaining -= (size_t)sent;
    }
    close(f);
    return 200;
}

// 循环读取，直到读完请求头 \r\n\r\n 或缓冲满或对端关闭
int read_http_request(int fd, char *buf, int max_len)
{
    int idx = 0;
    while (idx < max_len - 1)
    {
        int r = read(fd, buf + idx, max_len - 1 - idx);
        if (r < 0)
        {
            if (errno == EINTR)
                continue;
            return -1;
        }
        if (r == 0)
            break; // 对端关闭
        idx += r;
        buf[idx] = 0;
        if (strstr(buf, "\r\n\r\n") != NULL)
            break; // 请求头已读完
    }
    buf[idx] = 0;
    return idx;
}
