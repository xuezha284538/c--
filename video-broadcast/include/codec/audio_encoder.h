/*
 * audio_encoder.h - AAC 音频编码器 (FFmpeg)
 *
 * 使用 FFmpeg libavcodec 将 PCM 音频编码为 AAC
 */
#ifndef AUDIO_ENCODER_H
#define AUDIO_ENCODER_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct AudioEncoder AudioEncoder;

// 编码参数
typedef struct {
    int sample_rate;   // 采样率
    int channels;      // 通道数
    int bitrate;       // 比特率 (bps)
} AudioEncoderConfig;

// 创建编码器
AudioEncoder* audio_encoder_create(const AudioEncoderConfig *cfg);

// 销毁编码器
void audio_encoder_destroy(AudioEncoder *enc);

// 编码
// 返回 1 成功有输出, 0 成功无输出, -1 失败
int audio_encoder_encode(AudioEncoder *enc, const uint8_t *data, int frames,
                         int64_t pts, uint8_t *out_buf, size_t *out_size, int64_t *out_pts);

#ifdef __cplusplus
}
#endif

#endif // AUDIO_ENCODER_H
