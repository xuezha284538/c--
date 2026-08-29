/*
 * audio_capture.h - ALSA 音频采集
 *
 * 使用 Advanced Linux Sound Architecture 采集音频 PCM 数据
 */
#ifndef AUDIO_CAPTURE_H
#define AUDIO_CAPTURE_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct AudioCapture AudioCapture;

// 音频参数
typedef struct {
    const char *device;   // 设备名, 如 "default" / "hw:0,0"
    int sample_rate;      // 采样率 (默认 44100)
    int channels;         // 通道数 (默认 2)
    int sample_format;    // 采样格式 (默认 S16_LE)
    int period_size;      // 周期大小 (帧)
} AudioCaptureConfig;

// 打开音频设备
AudioCapture* audio_capture_open(const AudioCaptureConfig *cfg);

// 关闭音频设备
void audio_capture_close(AudioCapture *cap);

// 读取音频数据 (阻塞)
// 返回读取的帧数, -1 失败
int audio_capture_read(AudioCapture *cap, uint8_t *data, int frames, int64_t *pts);

// 获取采样率/通道数/格式
int audio_capture_sample_rate(AudioCapture *cap);
int audio_capture_channels(AudioCapture *cap);

#ifdef __cplusplus
}
#endif

#endif // AUDIO_CAPTURE_H
