/*
 * video_encoder.h - H.264 视频编码器 (FFmpeg)
 *
 * 使用 FFmpeg libavcodec 将原始 YUV 帧编码为 H.264
 */
#ifndef VIDEO_ENCODER_H
#define VIDEO_ENCODER_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct VideoEncoder VideoEncoder;

// 编码参数
typedef struct {
    int width;
    int height;
    int fps;
    int bitrate;       // 比特率 (bps)
    int gop_size;      // GOP 大小 (关键帧间隔)
    const char *preset; // 编码预设 (ultrafast / fast / medium / slow)
} VideoEncoderConfig;

// 创建编码器
VideoEncoder* video_encoder_create(const VideoEncoderConfig *cfg);

// 销毁编码器
void video_encoder_destroy(VideoEncoder *enc);

// 编码一帧
// data: YUV 数据, size: 数据大小, pts: 时间戳
// out_buf: 输出缓冲区, out_size: 输入缓冲大小/输出实际大小
// 返回 1 成功有输出, 0 成功无输出, -1 失败
int video_encoder_encode(VideoEncoder *enc, const uint8_t *data, size_t size,
                         int64_t pts, uint8_t *out_buf, size_t *out_size, int64_t *out_pts);

// 刷新编码器 (获取剩余输出)
int video_encoder_flush(VideoEncoder *enc, uint8_t *out_buf, size_t *out_size, int64_t *out_pts);

#ifdef __cplusplus
}
#endif

#endif // VIDEO_ENCODER_H
