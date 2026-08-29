/*
 * multicast_sender.c - UDP 组播发送器实现
 */
#include "transport/multicast_sender.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct MulticastSender {
    int fd;
    struct sockaddr_in addr;
    char multicast_addr[64];
    int port;
};

MulticastSender* multicast_sender_create(const MulticastSenderConfig *cfg) {
    if (!cfg || !cfg->multicast_addr || cfg->port <= 0) return NULL;

    MulticastSender *s = (MulticastSender*)calloc(1, sizeof(MulticastSender));
    if (!s) return NULL;

    s->port = cfg->port;
    strncpy(s->multicast_addr, cfg->multicast_addr, sizeof(s->multicast_addr) - 1);

    // 创建 UDP socket
    s->fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (s->fd < 0) {
        free(s);
        return NULL;
    }

    // 设置 TTL
    int ttl = cfg->ttl ? cfg->ttl : 64;
    setsockopt(s->fd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl));

    // 设置目标地址
    memset(&s->addr, 0, sizeof(s->addr));
    s->addr.sin_family = AF_INET;
    s->addr.sin_port = htons(cfg->port);
    inet_pton(AF_INET, cfg->multicast_addr, &s->addr.sin_addr);

    // 指定本地网卡 (可选)
    if (cfg->local_addr) {
        struct in_addr local_if;
        if (inet_pton(AF_INET, cfg->local_addr, &local_if) == 1) {
            setsockopt(s->fd, IPPROTO_IP, IP_MULTICAST_IF,
                       &local_if, sizeof(local_if));
        }
    }

    return s;
}

void multicast_sender_destroy(MulticastSender *s) {
    if (!s) return;
    if (s->fd >= 0) close(s->fd);
    free(s);
}

int multicast_sender_send(MulticastSender *s, const uint8_t *data, size_t size) {
    if (!s || !data || size == 0) return -1;

    return sendto(s->fd, data, size, 0,
                  (struct sockaddr*)&s->addr, sizeof(s->addr));
}

int multicast_sender_fd(MulticastSender *s) {
    return s ? s->fd : -1;
}
