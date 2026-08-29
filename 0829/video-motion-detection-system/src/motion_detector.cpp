/*
 * motion_detector.cpp - 运动目标检测器实现
 */
#include "motion_detector.h"

MotionDetector::MotionDetector()
    : mode_(MODE_MOG2)
    , diff_threshold_(17)
    , min_area_(900)
    , first_frame_(true)
{
    mog2_ = cv::createBackgroundSubtractorMOG2();
    kernel_ = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
}

std::string MotionDetector::mode_name() const {
    switch (mode_) {
        case MODE_FRAME_DIFF: return "FrameDiff";
        case MODE_MOG2:       return "MOG2";
        default:              return "Unknown";
    }
}

DetectResult MotionDetector::detect(const cv::Mat &frame) {
    switch (mode_) {
        case MODE_FRAME_DIFF: return detect_frame_diff(frame);
        case MODE_MOG2:       return detect_mog2(frame);
        default:              return DetectResult();
    }
}

// ---- 帧差法: 灰度化 + 高斯模糊 + 差分 + 二值化 ----
DetectResult MotionDetector::detect_frame_diff(const cv::Mat &frame) {
    cv::Mat gray, diff;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(7, 7), 0);

    if (first_frame_) {
        prev_gray_ = gray.clone();
        first_frame_ = false;
        DetectResult r;
        r.target_count = 0;
        r.mask = cv::Mat::zeros(frame.size(), CV_8UC1);
        return r;
    }

    cv::absdiff(gray, prev_gray_, diff);
    cv::threshold(diff, diff, diff_threshold_, 255, cv::THRESH_BINARY);
    prev_gray_ = gray.clone();

    return post_process(diff);
}

// ---- MOG2: 混合高斯背景建模 ----
DetectResult MotionDetector::detect_mog2(const cv::Mat &frame) {
    cv::Mat mask;
    mog2_->apply(frame, mask);
    return post_process(mask);
}

// ---- 后处理: 形态学降噪 + 轮廓检测 + 面积过滤 ----
DetectResult MotionDetector::post_process(const cv::Mat &mask) {
    DetectResult result;
    result.mask = mask.clone();

    // 形态学处理: 腐蚀去噪点 + 膨胀填充空洞
    cv::erode(result.mask, result.mask, kernel_);
    cv::dilate(result.mask, result.mask, kernel_);

    // 轮廓检测
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(result.mask, contours,
                     cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // 面积过滤 + 边界框
    for (auto &c : contours) {
        if (cv::contourArea(c) < min_area_) continue;
        result.targets.push_back(cv::boundingRect(c));
    }
    result.target_count = (int)result.targets.size();
    return result;
}

// ---- 绘制检测结果 ----
void MotionDetector::draw_results(cv::Mat &frame, const DetectResult &result,
                                  const cv::Scalar &color) {
    for (auto &box : result.targets) {
        cv::rectangle(frame, box, color, 2);
        cv::putText(frame, "Motion",
                    cv::Point(box.x, box.y - 5),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, color, 1);
    }
}
