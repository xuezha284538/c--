/*
 * frame_queue.h - 线程安全帧队列(生产者-消费者模型)
 *
 * 采集线程(生产者)持续压入帧, 处理线程(消费者)取出处理
 * 使用互斥锁 + 条件变量实现同步, 队列上限防止内存溢出
 */
#ifndef FRAME_QUEUE_H
#define FRAME_QUEUE_H

#include <opencv2/opencv.hpp>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <cstddef>

class FrameQueue {
public:
    explicit FrameQueue(size_t max_size = 3);

    // 压入一帧(阻塞,直到队列有空间或停止)
    void push(cv::Mat frame);

    // 取出一帧(阻塞,直到有帧或停止)
    // 返回 false 表示队列已关闭且无数据
    bool pop(cv::Mat &frame);

    // 关闭队列, 唤醒所有等待线程
    void shutdown();

    // 是否处于运行状态
    bool is_running() const { return !stop_flag_.load(); }

private:
    std::queue<cv::Mat> queue_;
    mutable std::mutex mtx_;
    std::condition_variable cv_pro_;   // 生产者等待条件
    std::condition_variable cv_con_;   // 消费者等待条件
    size_t max_size_;
    std::atomic<bool> stop_flag_;
};

#endif // FRAME_QUEUE_H
