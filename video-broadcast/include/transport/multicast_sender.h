/*
 * multicast_sender.h - UDP 组播发送器
 *
 * 通过 UDP 组播发送 RTP 音视频流
 * 一份数据, 多客户端接收, 带宽不随人数增长
 */
#ifndef MULTICAST_SENDER_H
#define MULTICAST_SENDER_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MulticastSender MulticastSender;

// 组播参数
typedef struct {
    const char *multicast_addr;  // 组播地址 (如 239.255.0.1)
    int port;                    // 端口
    const char *local_addr;      // 本地网卡地址 (NULL 用默认)
    int ttl;                     // TTL (默认 64)
} MulticastSenderConfig;

// 创建组播发送器
MulticastSender* multicast_sender_create(const MulticastSenderConfig *cfg);

// 销毁
void multicast_sender_destroy(MulticastSender *s);

// 发送数据
// 返回发送字节数, -1 失败
int multicast_sender_send(MulticastSender *s, const uint8_t *data, size_t size);

// 获取 socket fd
int multicast_sender_fd(MulticastSender *s);

#ifdef __cplusplus
}
#endif

#endif // MULTICAST_SENDER_H
