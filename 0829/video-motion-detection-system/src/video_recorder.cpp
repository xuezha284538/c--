/*
 * video_recorder.cpp - 视频录制器实现
 */
#include "video_recorder.h"

VideoRecorder::VideoRecorder() {}

VideoRecorder::~VideoRecorder() {
    release();
}

std::string VideoRecorder::codec_name(RecordCodec codec) {
    switch (codec) {
        case CODEC_MP4V: return "MP4V (H.264 compatible)";
        case CODEC_AVC1: return "AVC1 (H.264)";
        case CODEC_HEVC: return "HEVC (H.265)";
        case CODEC_XVID: return "XVID";
        case CODEC_MJPG: return "MJPG";
        default:         return "Unknown";
    }
}

static int codec_to_fourcc(RecordCodec codec) {
    switch (codec) {
        case CODEC_MP4V: return cv::VideoWriter::fourcc('m', 'p', '4', 'v');
        case CODEC_AVC1: return cv::VideoWriter::fourcc('a', 'v', 'c', '1');
        case CODEC_HEVC: return cv::VideoWriter::fourcc('h', 'e', 'v', '1');
        case CODEC_XVID: return cv::VideoWriter::fourcc('X', 'V', 'I', 'D');
        case CODEC_MJPG: return cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
        default:         return cv::VideoWriter::fourcc('m', 'p', '4', 'v');
    }
}

bool VideoRecorder::open(const std::string &filename, int width, int height,
                         double fps, RecordCodec codec) {
    int fourcc = codec_to_fourcc(codec);
    bool ok = writer_.open(filename, fourcc, fps, cv::Size(width, height));
    if (ok) filename_ = filename;
    return ok;
}

void VideoRecorder::write(const cv::Mat &frame) {
    if (writer_.isOpened()) writer_.write(frame);
}

void VideoRecorder::release() {
    if (writer_.isOpened()) {
        writer_.release();
        filename_.clear();
    }
}
