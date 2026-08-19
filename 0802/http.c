#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/sendfile.h>
#include <errno.h>
#include <sys/time.h>
#include <netinet/tcp.h>

#define PORT 6816
#define LISTEN_BACKLOG 256
#define BUF_MAX 4096
#define SOCK_TIMEOUT_SEC 3

#ifndef SO_REUSEPORT
#define SO_REUSEPORT 15
#endif

// 线程参数结构体：存放fd、客户端ip、端口
typedef struct
{
    int c_fd;
    char ip[INET_ADDRSTRLEN];
    int port;
} ClientInfo;

const char *get_type(const char *path)
{
    if (strstr(path, ".html"))
        return "text/html";
    if (strstr(path, ".png"))
        return "image/png";
    if (strstr(path, ".jpg") || strstr(path, ".jpeg"))
        return "image/jpeg";
    if (strstr(path, ".gif"))
        return "image/gif";
    if (strstr(path, ".ico"))
        return "image/x-icon";
    if (strstr(path, ".mp4"))
        return "video/mp4";
    if (strstr(path, ".mp3"))
        return "audio/mpeg";
    if (strstr(path, ".m4a"))
        return "audio/mp4";
    if (strstr(path, ".pdf"))
        return "application/pdf";
    if (strstr(path, ".css"))
        return "text/css";
    if (strstr(path, ".js"))
        return "application/javascript";
    if (strstr(path, ".txt"))
        return "text/plain";
    return "application/octet-stream";
}

static int read_http_header(int fd, char *buf, int max_len)
{
    int idx = 0;
    while (idx < max_len - 4)
    {
        int read_len = read(fd, buf + idx, max_len - idx - 4);
        if (read_len < 0)
        {
            if (errno == EINTR)
                continue;
            return -1;
        }
        if (read_len == 0)
            return -1;
        idx += read_len;
        buf[idx] = '\0';
        if (strstr(buf, "\r\n\r\n") != NULL)
            break;
    }
    return idx;
}

void *handle(void *arg)
{
    ClientInfo *info = (ClientInfo *)arg;
    int c_fd = info->c_fd;
    char *client_ip = info->ip;
    int client_port = info->port;
    free(arg);

    struct timeval timeout = {SOCK_TIMEOUT_SEC, 0};
    setsockopt(c_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    setsockopt(c_fd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));

    int nodelay = 1;
    setsockopt(c_fd, IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof(nodelay));

    char buf[BUF_MAX] = {0};
    int hdr_len = read_http_header(c_fd, buf, BUF_MAX);
    if (hdr_len <= 0)
    {
        // 连接异常断开，依旧打印来源
        printf("[%s:%d] 连接异常断开，读取请求失败\n", client_ip, client_port);
        close(c_fd);
        return NULL;
    }

    printf("[%s:%d] 收到完整请求:\n%s\n", client_ip, client_port, buf);

    char method[16], path[512], version[16];
    if (sscanf(buf, "%s %s %s", method, path, version) != 3)
    {
        close(c_fd);
        return NULL;
    }
    if (strcmp(method, "GET") != 0)
    {
        close(c_fd);
        return NULL;
    }

    char filepath[1024];
    if (strcmp(path, "/") == 0)
        strcpy(path, "/index.html");
    snprintf(filepath, sizeof(filepath), "html%s", path);

    if (strstr(filepath, "..") || strstr(filepath, "/./") || strstr(filepath, "/../"))
    {
        close(c_fd);
        return NULL;
    }

    int file_fd = open(filepath, O_RDONLY);
    if (file_fd < 0)
    {
        const char *body = "404 Not Found";
        char resp[512];
        int len = snprintf(resp, sizeof(resp),
                           "HTTP/1.1 404 Not Found\r\n"
                           "Content-Type: text/plain\r\n"
                           "Content-Length: %zu\r\n"
                           "Connection: close\r\n\r\n%s",
                           strlen(body), body);
        write(c_fd, resp, len);
        close(c_fd);
        return NULL;
    }

    off_t fsize = lseek(file_fd, 0, SEEK_END);
    lseek(file_fd, 0, SEEK_SET);
    const char *ctype = get_type(filepath);

    char header[512];
    int hlen = snprintf(header, sizeof(header),
                        "HTTP/1.1 200 OK\r\n"
                        "Content-Type: %s\r\n"
                        "Content-Length: %lld\r\n"
                        "Connection: close\r\n\r\n",
                        ctype, (long long)fsize);
    write(c_fd, header, hlen);

    off_t offset = 0;
    ssize_t send_len;
    while ((send_len = sendfile(c_fd, file_fd, &offset, fsize - offset)) > 0)
        ;

    close(file_fd);
    close(c_fd);
    return NULL;
}

int main(void)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket create failed");
        return 1;
    }

    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("bind failed");
        close(sockfd);
        return 1;
    }

    if (listen(sockfd, LISTEN_BACKLOG) < 0)
    {
        perror("listen failed");
        close(sockfd);
        return 1;
    }

    printf("HTTP Server running on 0.0.0.0:%d\n", PORT);

    while (1)
    {
        struct sockaddr_in cli_addr;
        socklen_t cli_len = sizeof(cli_addr);
        int c_fd = accept(sockfd, (struct sockaddr *)&cli_addr, &cli_len);

        if (c_fd < 0)
        {
            if (errno == EINTR)
                continue;
            perror("accept");
            continue;
        }

        // accept成功立刻解析ip端口，提前保存
        ClientInfo *info = malloc(sizeof(ClientInfo));
        info->c_fd = c_fd;
        inet_ntop(AF_INET, &cli_addr.sin_addr, info->ip, INET_ADDRSTRLEN);
        info->port = ntohs(cli_addr.sin_port);

        pthread_t tid;
        pthread_create(&tid, NULL, handle, info);
        pthread_detach(tid);
    }

    close(sockfd);
    return 0;
}