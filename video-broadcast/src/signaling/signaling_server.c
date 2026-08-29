/*
 * signaling_server.c - TCP 信令服务器 (框架实现)
 *
 * TODO: 完整实现 epoll 多路IO复用的 TCP 服务器
 *   1. socket → bind → listen → epoll_create1 → epoll_ctl
 *   2. epoll_wait 事件循环
 *   3. accept 新连接 → 加入 epoll
 *   4. 数据可读 → 读消息 → 回调处理
 *   5. 客户端断开 → 清理 + 从 epoll 移除
 */
#include "signaling/signaling_server.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <signal.h>

#define MAX_CLIENTS 256
#define MAX_EVENTS 128

struct SignalingServer {
    int listen_fd;
    int epfd;
    int port;
    int running;
    pthread_t thread;

    sig_client_cb on_join;
    sig_client_cb on_leave;
    sig_message_cb on_message;
    void *user_data;

    // 客户端表
    int clients[MAX_CLIENTS];
    int client_count;
};

SignalingServer* sig_server_create(int port) {
    if (port <= 0) return NULL;

    SignalingServer *srv = (SignalingServer*)calloc(1, sizeof(SignalingServer));
    if (!srv) return NULL;

    srv->port = port;
    srv->running = 0;
    srv->client_count = 0;
    for (int i = 0; i < MAX_CLIENTS; i++) srv->clients[i] = -1;

    // 创建 listen socket
    srv->listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (srv->listen_fd < 0) {
        free(srv);
        return NULL;
    }

    int opt = 1;
    setsockopt(srv->listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(srv->listen_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        close(srv->listen_fd);
        free(srv);
        return NULL;
    }

    if (listen(srv->listen_fd, 128) < 0) {
        close(srv->listen_fd);
        free(srv);
        return NULL;
    }

    // epoll
    srv->epfd = epoll_create1(0);
    if (srv->epfd < 0) {
        close(srv->listen_fd);
        free(srv);
        return NULL;
    }

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = srv->listen_fd;
    epoll_ctl(srv->epfd, EPOLL_CTL_ADD, srv->listen_fd, &ev);

    return srv;
}

void sig_server_destroy(SignalingServer *srv) {
    if (!srv) return;
    sig_server_stop(srv);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (srv->clients[i] >= 0) close(srv->clients[i]);
    }
    if (srv->epfd >= 0) close(srv->epfd);
    if (srv->listen_fd >= 0) close(srv->listen_fd);
    free(srv);
}

void sig_server_set_callbacks(SignalingServer *srv,
                               sig_client_cb on_join,
                               sig_client_cb on_leave,
                               sig_message_cb on_message,
                               void *user_data) {
    if (!srv) return;
    srv->on_join = on_join;
    srv->on_leave = on_leave;
    srv->on_message = on_message;
    srv->user_data = user_data;
}

static void* server_thread(void *arg) {
    SignalingServer *srv = (SignalingServer*)arg;
    struct epoll_event events[MAX_EVENTS];

    // TODO: 完整 epoll 事件循环 (accept / recv / 消息解析 / 回调)
    // 框架占位
    while (srv->running) {
        int n = epoll_wait(srv->epfd, events, MAX_EVENTS, 100);
        if (n < 0 && errno != EINTR) break;
        if (n <= 0) continue;

        for (int i = 0; i < n; i++) {
            int fd = events[i].data.fd;
            if (fd == srv->listen_fd) {
                // 新连接
                struct sockaddr_in caddr;
                socklen_t len = sizeof(caddr);
                int cfd = accept(fd, (struct sockaddr*)&caddr, &len);
                if (cfd < 0) continue;

                // 加入 epoll
                struct epoll_event ev;
                ev.events = EPOLLIN | EPOLLRDHUP;
                ev.data.fd = cfd;
                epoll_ctl(srv->epfd, EPOLL_CTL_ADD, cfd, &ev);

                // 记录客户端
                for (int j = 0; j < MAX_CLIENTS; j++) {
                    if (srv->clients[j] < 0) {
                        srv->clients[j] = cfd;
                        srv->client_count++;
                        break;
                    }
                }

                // 回调
                if (srv->on_join) {
                    char ip[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &caddr.sin_addr, ip, sizeof(ip));
                    srv->on_join(cfd, ip, ntohs(caddr.sin_port), srv->user_data);
                }
            } else {
                // 客户端数据 (TODO: 完整消息解析)
                char buf[1024];
                int n = recv(fd, buf, sizeof(buf), 0);
                if (n <= 0) {
                    // 断开
                    close(fd);
                    for (int j = 0; j < MAX_CLIENTS; j++) {
                        if (srv->clients[j] == fd) {
                            srv->clients[j] = -1;
                            srv->client_count--;
                            if (srv->on_leave)
                                srv->on_leave(fd, "", 0, srv->user_data);
                            break;
                        }
                    }
                }
                // TODO: 解析消息头 (type + length) + 回调 on_message
            }
        }
    }

    return NULL;
}

int sig_server_run(SignalingServer *srv) {
    if (!srv || srv->running) return -1;
    srv->running = 1;
    return pthread_create(&srv->thread, NULL, server_thread, srv);
}

void sig_server_stop(SignalingServer *srv) {
    if (!srv || !srv->running) return;
    srv->running = 0;
    pthread_join(srv->thread, NULL);
}

int sig_server_send(SignalingServer *srv, int client_fd,
                    SigMsgType type, const uint8_t *data, size_t len) {
    if (!srv || client_fd < 0) return -1;

    // TODO: 构造消息头 + 消息体, 发送
    // 简化实现: 直接发送
    uint32_t header[2] = { (uint32_t)type, (uint32_t)len };
    send(client_fd, header, sizeof(header), 0);
    if (data && len > 0) send(client_fd, data, len, 0);
    return 0;
}

int sig_server_broadcast(SignalingServer *srv,
                         SigMsgType type, const uint8_t *data, size_t len) {
    if (!srv) return -1;
    int count = 0;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (srv->clients[i] >= 0) {
            sig_server_send(srv, srv->clients[i], type, data, len);
            count++;
        }
    }
    return count;
}
