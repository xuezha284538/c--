/*
 * motion_detector.h - 运动目标检测器
 *
 * 支持两种检测模式:
 *   - MODE_FRAME_DIFF: 帧差法
 *     代码量小、计算量低, 适合嵌入式等资源受限场景
 *     原理: 前后帧灰度差分 + 阈值二值化 + 形态学处理 + 轮廓检测
 *
 *   - MODE_MOG2: MOG2 混合高斯背景建模
 *     精度更高, 适应光照变化与动态背景, 但计算量更大
 *     原理: 每个像素维护多个高斯分布, 新帧匹配则为背景, 不匹配则为前景
 */
#ifndef MOTION_DETECTOR_H
#define MOTION_DETECTOR_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

enum DetectMode {
    MODE_FRAME_DIFF = 0,   // 帧差法(轻量,适合嵌入式)
    MODE_MOG2 = 1          // MOG2混合高斯背景建模(精度高)
};

struct DetectResult {
    std::vector<cv::Rect> targets;  // 检测到的目标边界框
    cv::Mat mask;                    // 前景掩码(二值图)
    int target_count;                // 目标数量
};

class MotionDetector {
public:
    MotionDetector();

    // 设置检测模式
    void set_mode(DetectMode mode) { mode_ = mode; }
    DetectMode mode() const { return mode_; }

    // 设置帧差法阈值(默认17)
    void set_diff_threshold(int thresh) { diff_threshold_ = thresh; }

    // 设置最小目标面积(默认900)
    void set_min_area(int area) { min_area_ = area; }

    // 检测一帧, 返回检测结果
    DetectResult detect(const cv::Mat &frame);

    // 在帧上绘制检测结果
    void draw_results(cv::Mat &frame, const DetectResult &result,
                      const cv::Scalar &color = cv::Scalar(0, 255, 0));

    // 获取模式名称字符串
    std::string mode_name() const;

private:
    DetectMode mode_;
    int diff_threshold_;
    int min_area_;

    // 帧差法相关
    cv::Mat prev_gray_;
    bool first_frame_;

    // MOG2 背景减法器
    cv::Ptr<cv::BackgroundSubtractor> mog2_;

    // 形态学核
    cv::Mat kernel_;

    // 帧差法检测
    DetectResult detect_frame_diff(const cv::Mat &frame);

    // MOG2 检测
    DetectResult detect_mog2(const cv::Mat &frame);

    // 公共后处理: 形态学 + 轮廓检测 + 面积过滤
    DetectResult post_process(const cv::Mat &mask);
};

#endif // MOTION_DETECTOR_H
