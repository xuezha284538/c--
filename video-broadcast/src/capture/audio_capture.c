/*
 * audio_capture.c - ALSA 音频采集 (框架实现)
 *
 * TODO: 完整实现 ALSA PCM 采集流程
 *   1. snd_pcm_open → snd_pcm_set_params → snd_pcm_prepare
 *   2. snd_pcm_readi 读数据
 *   3. snd_pcm_drain → snd_pcm_close
 */
#include "capture/audio_capture.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#ifdef HAVE_ALSA
#include <alsa/asoundlib.h>
#endif

struct AudioCapture {
#ifdef HAVE_ALSA
    snd_pcm_t *handle;
    snd_pcm_uframes_t period_size;
#else
    unsigned long period_size;
#endif
    int sample_rate;
    int channels;
    int sample_format;
    int bytes_per_frame;
};

AudioCapture* audio_capture_open(const AudioCaptureConfig *cfg) {
    if (!cfg) return NULL;

    AudioCapture *cap = (AudioCapture*)calloc(1, sizeof(AudioCapture));
    if (!cap) return NULL;

    cap->sample_rate = cfg->sample_rate ? cfg->sample_rate : 44100;
    cap->channels = cfg->channels ? cfg->channels : 2;
#ifdef HAVE_ALSA
    cap->sample_format = SND_PCM_FORMAT_S16_LE;
#else
    cap->sample_format = 0; // S16_LE placeholder
#endif
    cap->bytes_per_frame = cap->channels * 2; // S16_LE = 2 bytes/sample

#ifdef HAVE_ALSA
    // 完整 ALSA 初始化 (open / set_params / prepare)
    int err = snd_pcm_open(&cap->handle, cfg->device ? cfg->device : "default",
                           SND_PCM_STREAM_CAPTURE, 0);
    if (err < 0) {
        free(cap);
        return NULL;
    }
#else
    // 无 ALSA, 返回占位实现
    (void)cfg;
#endif

    return cap;
}

void audio_capture_close(AudioCapture *cap) {
    if (!cap) return;
#ifdef HAVE_ALSA
    if (cap->handle) {
        snd_pcm_drain(cap->handle);
        snd_pcm_close(cap->handle);
    }
#endif
    free(cap);
}

int audio_capture_read(AudioCapture *cap, uint8_t *data, int frames, int64_t *pts) {
    if (!cap || !data || frames <= 0) return -1;

    // TODO: snd_pcm_readi 真实读取
    // 目前返回静音
    memset(data, 0, frames * cap->bytes_per_frame);

    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    if (pts) *pts = (int64_t)ts.tv_sec * 1000000 + ts.tv_nsec / 1000;

    return frames;
}

int audio_capture_sample_rate(AudioCapture *cap) {
    return cap ? cap->sample_rate : 0;
}

int audio_capture_channels(AudioCapture *cap) {
    return cap ? cap->channels : 0;
}
