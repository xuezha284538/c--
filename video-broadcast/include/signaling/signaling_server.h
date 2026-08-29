/*
 * signaling_server.h - TCP 信令服务器
 *
 * 独立的 TCP 通道处理客户端加入/离开/控制指令
 * 与媒体流通道分离, 保证信令可靠传输
 */
#ifndef SIGNALING_SERVER_H
#define SIGNALING_SERVER_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SignalingServer SignalingServer;

// 信令消息类型
typedef enum {
    SIG_JOIN = 1,       // 客户端请求加入
    SIG_LEAVE = 2,      // 客户端离开
    SIG_INFO = 3,       // 服务器发送流信息 (SDP/参数)
    SIG_CONTROL = 4,    // 控制指令 (暂停/恢复等)
    SIG_KEEPALIVE = 5   // 心跳
} SigMsgType;

// 信令消息
typedef struct {
    uint32_t type;      // 消息类型 (SigMsgType)
    uint32_t length;    // 消息体长度
    uint8_t *payload;   // 消息体
} SigMessage;

// 客户端连接回调
typedef void (*sig_client_cb)(int client_fd, const char *ip, int port, void *user_data);

// 消息回调
typedef void (*sig_message_cb)(int client_fd, const SigMessage *msg, void *user_data);

// 创建信令服务器
SignalingServer* sig_server_create(int port);

// 销毁
void sig_server_destroy(SignalingServer *srv);

// 设置回调
void sig_server_set_callbacks(SignalingServer *srv,
                               sig_client_cb on_join,
                               sig_client_cb on_leave,
                               sig_message_cb on_message,
                               void *user_data);

// 运行服务器 (阻塞, 在单独线程中调用)
int sig_server_run(SignalingServer *srv);

// 停止服务器
void sig_server_stop(SignalingServer *srv);

// 向指定客户端发送消息
int sig_server_send(SignalingServer *srv, int client_fd,
                    SigMsgType type, const uint8_t *data, size_t len);

// 广播消息给所有客户端
int sig_server_broadcast(SignalingServer *srv,
                         SigMsgType type, const uint8_t *data, size_t len);

#ifdef __cplusplus
}
#endif

#endif // SIGNALING_SERVER_H
