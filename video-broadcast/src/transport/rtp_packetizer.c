/*
 * rtp_packetizer.c - RTP 打包器实现 (框架)
 *
 * TODO: 完整实现 H.264 RTP 打包 (RFC 3984)
 *   - Single NAL unit mode (单个NAL小于MTU)
 *   - FU-A fragmentation mode (单个NAL大于MTU时分片)
 *   - STAP-A aggregation mode (多个小NAL合并)
 */
#include "transport/rtp_packetizer.h"
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

void rtp_packetizer_init(RtpPacketizer *p, uint8_t pt, uint32_t ssrc, uint32_t clock_rate) {
    if (!p) return;
    p->ssrc = ssrc;
    p->seq = 0;
    p->payload_type = pt;
    p->clock_rate = clock_rate;
}

static void fill_rtp_header(RtpHeader *hdr, RtpPacketizer *p, int marker, uint32_t rtp_ts) {
    hdr->v_p_x_cc = (RTP_VERSION << 6);  // V=2, P=0, X=0, CC=0
    hdr->m_pt = (marker ? 0x80 : 0) | (p->payload_type & 0x7F);
    hdr->seq = htons(p->seq++);
    hdr->timestamp = htonl(rtp_ts);
    hdr->ssrc = htonl(p->ssrc);
}

uint16_t rtp_packetizer_next_seq(RtpPacketizer *p) {
    return p ? p->seq : 0;
}

int rtp_packetizer_packetize(RtpPacketizer *p, const uint8_t *data, size_t size,
                             int64_t pts_us, uint8_t *out_packets, int max_packets,
                             size_t *packet_sizes) {
    if (!p || !data || !out_packets || !packet_sizes || max_packets <= 0)
        return -1;

    // RTP 时间戳 = 微秒时间戳 * 时钟频率 / 1e6
    uint32_t rtp_ts = (uint32_t)((int64_t)pts_us * p->clock_rate / 1000000);

    // TODO: 完整 RTP 打包 (H.264 FU-A 分片 / STAP-A 聚合等)
    // 目前简化: 整帧打成一个包 (仅占位, 实际需按MTU分片)
    size_t packet_size = RTP_HEADER_SIZE + size;
    if (packet_sizes[0] < packet_size && max_packets >= 1) {
        // 输出缓冲区不够的情况, 这里假设足够大
    }

    RtpHeader *hdr = (RtpHeader*)out_packets;
    fill_rtp_header(hdr, p, 1, rtp_ts); // M=1 标记帧结束
    memcpy(out_packets + RTP_HEADER_SIZE, data, size);
    packet_sizes[0] = packet_size;

    return 1; // 1 个包
}
