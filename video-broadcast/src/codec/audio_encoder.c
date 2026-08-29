/*
 * audio_encoder.c - AAC 音频编码器 (框架实现)
 *
 * TODO: 完整 FFmpeg AAC 编码流程
 *   1. avcodec_find_encoder(AV_CODEC_ID_AAC) → 设置参数 → avcodec_open2
 *   2. av_frame_alloc + av_samples_fill_arrays 设置帧
 *   3. avcodec_send_frame + avcodec_receive_packet
 *   4. 释放资源
 */
#include "codec/audio_encoder.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <libavcodec/avcodec.h>

struct AudioEncoder {
    const AVCodec *codec;
    AVCodecContext *ctx;
    AVFrame *frame;
    AVPacket *pkt;
    int sample_rate;
    int channels;
};

AudioEncoder* audio_encoder_create(const AudioEncoderConfig *cfg) {
    if (!cfg) return NULL;

    AudioEncoder *enc = (AudioEncoder*)calloc(1, sizeof(AudioEncoder));
    if (!enc) return NULL;

    enc->sample_rate = cfg->sample_rate ? cfg->sample_rate : 44100;
    enc->channels = cfg->channels ? cfg->channels : 2;

    // TODO: 完整 FFmpeg AAC 编码器初始化
    enc->codec = avcodec_find_encoder(AV_CODEC_ID_AAC);
    if (!enc->codec) {
        free(enc);
        return NULL;
    }

    return enc;
}

void audio_encoder_destroy(AudioEncoder *enc) {
    if (!enc) return;
    // TODO: 释放 FFmpeg 资源
    free(enc);
}

int audio_encoder_encode(AudioEncoder *enc, const uint8_t *data, int frames,
                         int64_t pts, uint8_t *out_buf, size_t *out_size, int64_t *out_pts) {
    if (!enc || !data || !out_buf || !out_size) return -1;

    // TODO: 完整 FFmpeg AAC 编码
    // 占位: 直接复制
    size_t data_size = frames * enc->channels * 2; // S16
    if (*out_size < data_size) return -1;
    memcpy(out_buf, data, data_size);
    *out_size = data_size;
    if (out_pts) *out_pts = pts;
    return 1;
}

#ifdef __cplusplus
}
#endif
