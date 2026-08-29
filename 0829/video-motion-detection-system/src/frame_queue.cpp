/*
 * frame_queue.cpp - 线程安全帧队列实现
 */
#include "frame_queue.h"

FrameQueue::FrameQueue(size_t max_size)
    : max_size_(max_size), stop_flag_(false) {}

void FrameQueue::push(cv::Mat frame) {
    std::unique_lock<std::mutex> lock(mtx_);
    cv_pro_.wait(lock, [this] {
        return queue_.size() < max_size_ || stop_flag_.load();
    });
    if (stop_flag_.load()) return;
    queue_.push(std::move(frame));
    cv_con_.notify_one();
}

bool FrameQueue::pop(cv::Mat &frame) {
    std::unique_lock<std::mutex> lock(mtx_);
    cv_con_.wait(lock, [this] {
        return !queue_.empty() || stop_flag_.load();
    });
    if (queue_.empty() && stop_flag_.load()) return false;
    frame = std::move(queue_.front());
    queue_.pop();
    cv_pro_.notify_one();
    return true;
}

void FrameQueue::shutdown() {
    stop_flag_.store(true);
    cv_con_.notify_all();
    cv_pro_.notify_all();
}
