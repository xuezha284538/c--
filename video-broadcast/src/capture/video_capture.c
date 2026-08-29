/*
 * video_capture.c - V4L2 视频采集 (框架实现)
 *
 * TODO: 完整实现 V4L2 mmap 采集流程
 *   1. open设备 → ioctl查询能力 → 设置格式 → 请求缓冲区 → mmap映射 → 入队 → 启动流
 *   2. dequeue 取帧 → 处理 → enqueue 归还
 *   3. 停止流 → munmap → 关闭
 */
#include "capture/video_capture.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <time.h>

#define MAX_BUFFERS 4

struct VideoCapture {
    int fd;
    int width;
    int height;
    int fps;
    int pixel_format;
    size_t buffer_size;

    // mmap 缓冲区
    void *buffers[MAX_BUFFERS];
    size_t buffer_lengths[MAX_BUFFERS];
    int buffer_count;
};

VideoCapture* video_capture_open(const VideoCaptureConfig *cfg) {
    if (!cfg || !cfg->device) return NULL;

    VideoCapture *cap = (VideoCapture*)calloc(1, sizeof(VideoCapture));
    if (!cap) return NULL;

    cap->width = cfg->width ? cfg->width : 640;
    cap->height = cfg->height ? cfg->height : 480;
    cap->fps = cfg->fps ? cfg->fps : 30;
    cap->pixel_format = cfg->pixel_format ? cfg->pixel_format : V4L2_PIX_FMT_YUYV;
    cap->buffer_size = cap->width * cap->height * 2; // YUYV = 2 bytes/pixel

    // TODO: 完整 V4L2 初始化 (open / set_fmt / reqbufs / mmap / streamon)
    // 目前仅占位, 实际项目中实现完整采集流程
    cap->fd = open(cfg->device, O_RDWR);
    if (cap->fd < 0) {
        free(cap);
        return NULL;
    }

    return cap;
}

void video_capture_close(VideoCapture *cap) {
    if (!cap) return;
    // TODO: munmap buffers / streamoff / close
    if (cap->fd >= 0) close(cap->fd);
    free(cap);
}

int video_capture_get_frame(VideoCapture *cap, uint8_t *data, size_t *size, int64_t *pts) {
    if (!cap || !data || !size) return -1;

    // TODO: 完整 V4L2 dqbuf/处理/qbuf 流程
    // 目前返回模拟数据
    if (*size < cap->buffer_size) return -1;

    memset(data, 0, cap->buffer_size);
    *size = cap->buffer_size;

    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    if (pts) *pts = (int64_t)ts.tv_sec * 1000000 + ts.tv_nsec / 1000;

    return 0;
}

size_t video_capture_buffer_size(VideoCapture *cap) {
    return cap ? cap->buffer_size : 0;
}

int video_capture_width(VideoCapture *cap) {
    return cap ? cap->width : 0;
}

int video_capture_height(VideoCapture *cap) {
    return cap ? cap->height : 0;
}

int video_capture_fps(VideoCapture *cap) {
    return cap ? cap->fps : 0;
}
