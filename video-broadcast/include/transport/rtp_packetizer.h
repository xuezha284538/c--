/*
 * rtp_packetizer.h - RTP 打包器
 *
 * 将编码后的音视频数据封装为 RTP 包
 * RTP 头: 版本(2) + 填充 + 扩展 + CSRC数 + 标记 + PT + 序号 + 时间戳 + SSRC
 */
#ifndef RTP_PACKETIZER_H
#define RTP_PACKETIZER_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// RTP 固定头 (12字节)
typedef struct {
    // 第1字节: V=2, P, X, CC
    uint8_t v_p_x_cc;
    // 第2字节: M, PT
    uint8_t m_pt;
    uint16_t seq;       // 序列号 (网络字节序)
    uint32_t timestamp; // 时间戳 (网络字节序)
    uint32_t ssrc;      // 同步源标识 (网络字节序)
} __attribute__((packed)) RtpHeader;

#define RTP_HEADER_SIZE 12
#define RTP_VERSION 2
#define RTP_MAX_PAYLOAD 1400  // 小于 MTU 避免分片

// 负载类型
#define RTP_PT_H264 96   // 动态 PT, 视频 H.264
#define RTP_PT_AAC  97   // 动态 PT, 音频 AAC

typedef struct RtpPacketizer {
    uint32_t ssrc;
    uint16_t seq;
    uint8_t payload_type;
    uint32_t clock_rate;  // 时钟频率 (视频90000, 音频按采样率)
} RtpPacketizer;

// 初始化打包器
void rtp_packetizer_init(RtpPacketizer *p, uint8_t pt, uint32_t ssrc, uint32_t clock_rate);

// 打包一帧 (可能输出多个 RTP 包)
// data: 编码数据, size: 数据大小, pts: 时间戳(微秒)
// out_packets: 输出 RTP 包数组, max_packets: 最大包数
// 返回实际生成的包数, -1 失败
int rtp_packetizer_packetize(RtpPacketizer *p, const uint8_t *data, size_t size,
                             int64_t pts_us, uint8_t *out_packets, int max_packets,
                             size_t *packet_sizes);

// 获取下一个序列号
uint16_t rtp_packetizer_next_seq(RtpPacketizer *p);

#ifdef __cplusplus
}
#endif

#endif // RTP_PACKETIZER_H
