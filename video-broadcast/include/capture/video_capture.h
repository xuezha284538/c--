/*
 * video_capture.h - V4L2 视频采集
 *
 * 使用 Video4Linux2 API 采集摄像头原始 YUV/RGB 数据
 */
#ifndef VIDEO_CAPTURE_H
#define VIDEO_CAPTURE_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct VideoCapture VideoCapture;

// 采集参数
typedef struct {
    const char *device;   // 设备路径, 如 /dev/video0
    int width;            // 宽度
    int height;           // 高度
    int fps;              // 帧率
    int pixel_format;     // 像素格式 (V4L2_PIX_FMT_YUYV 等)
} VideoCaptureConfig;

// 打开摄像头
VideoCapture* video_capture_open(const VideoCaptureConfig *cfg);

// 关闭摄像头
void video_capture_close(VideoCapture *cap);

// 获取一帧 (阻塞)
// 返回 0 成功, -1 失败
int video_capture_get_frame(VideoCapture *cap, uint8_t *data, size_t *size, int64_t *pts);

// 获取缓冲区大小
size_t video_capture_buffer_size(VideoCapture *cap);

// 获取宽度/高度/帧率
int video_capture_width(VideoCapture *cap);
int video_capture_height(VideoCapture *cap);
int video_capture_fps(VideoCapture *cap);

#ifdef __cplusplus
}
#endif

#endif // VIDEO_CAPTURE_H
