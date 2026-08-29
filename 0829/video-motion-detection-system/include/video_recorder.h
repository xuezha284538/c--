/*
 * video_recorder.h - 视频录制器
 *
 * 封装 VideoWriter, 支持多种编码格式:
 *   - H.264 (mp4v / avc1)
 *   - H.265 (hev1)
 *   - AVI  (XVID / MJPG)
 */
#ifndef VIDEO_RECORDER_H
#define VIDEO_RECORDER_H

#include <opencv2/opencv.hpp>
#include <string>

enum RecordCodec {
    CODEC_MP4V = 0,   // H.264 兼容 (mp4v)
    CODEC_AVC1 = 1,   // H.264 (avc1)
    CODEC_HEVC = 2,   // H.265 (hev1)
    CODEC_XVID = 3,   // XVID (avi)
    CODEC_MJPG = 4    // MJPG (avi)
};

class VideoRecorder {
public:
    VideoRecorder();
    ~VideoRecorder();

    // 打开录制文件
    // fps: 录制帧率, codec: 编码格式
    bool open(const std::string &filename, int width, int height,
              double fps = 15.0, RecordCodec codec = CODEC_MP4V);

    // 写入一帧
    void write(const cv::Mat &frame);

    // 是否处于录制状态
    bool is_recording() const { return writer_.isOpened(); }

    // 关闭录制
    void release();

    // 获取编码格式名称
    static std::string codec_name(RecordCodec codec);

private:
    cv::VideoWriter writer_;
    std::string filename_;
};

#endif // VIDEO_RECORDER_H
