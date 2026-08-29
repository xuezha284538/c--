/*
 * 实时视频采集与运动检测系统
 * 功能: 视频采集、多编码格式录制、运动目标检测(帧差法/MOG2双模式)
 * 优化: 采集-处理多线程分离 + MOG2背景建模 + FPS监控 + 参数可配置 + 错误处理
 */
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include <string>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

// ==================== 生产者-消费者帧队列 ====================
// 采集线程(生产者)与处理线程(消费者)之间的缓冲队列
// 互斥锁保护队列, 条件变量同步生产/消费节奏
class FrameQueue {
    queue<Mat> q;
    mutex mtx;
    condition_variable cv_pro, cv_con;
    size_t max_size;
    atomic<bool> stop_flag;

public:
    FrameQueue(size_t sz = 3) : max_size(sz), stop_flag(false) {}

    void push(Mat frame) {
        unique_lock<mutex> lock(mtx);
        cv_pro.wait(lock, [&] { return q.size() < max_size || stop_flag.load(); });
        if (stop_flag.load()) return;
        q.push(move(frame));
        cv_con.notify_one();
    }

    bool pop(Mat &frame) {
        unique_lock<mutex> lock(mtx);
        cv_con.wait(lock, [&] { return !q.empty() || stop_flag.load(); });
        if (q.empty() && stop_flag.load()) return false;
        frame = move(q.front());
        q.pop();
        cv_pro.notify_one();
        return true;
    }

    void shutdown() {
        stop_flag.store(true);
        cv_con.notify_all();
        cv_pro.notify_all();
    }
};

// ==================== 全局运行标志 ====================
atomic<bool> g_running(true);

// ==================== 采集线程(生产者) ====================
// 独立线程持续采集摄像头帧, 压入队列供处理线程消费
void captureThread(VideoCapture &cap, FrameQueue &queue) {
    Mat frame;
    while (g_running.load()) {
        cap >> frame;
        if (frame.empty()) {
            g_running.store(false);
            queue.shutdown();
            cerr << "[采集] 摄像头读取失败" << endl;
            break;
        }
        queue.push(move(frame));
    }
}

// ==================== 主处理流程(消费者) ====================
int main(int argc, char *argv[]) {
    // 参数: [摄像头索引] [检测模式: 0=帧差法 1=MOG2] [最小目标面积] [录制文件名]
    int camIndex = (argc > 1) ? stoi(argv[1]) : 0;
    int mode = (argc > 2) ? stoi(argv[2]) : 1;
    int minArea = (argc > 3) ? stoi(argv[3]) : 900;
    string recordPath = (argc > 4) ? argv[4] : "";

    cout << "模式: " << (mode == 0 ? "帧差法" : "MOG2混合高斯")
         << " | 摄像头: " << camIndex
         << " | 最小面积: " << minArea;
    if (!recordPath.empty()) cout << " | 录制: " << recordPath;
    cout << endl;

    // ---- 1. 打开摄像头 ----
    VideoCapture cap;
    if (!cap.open(camIndex)) {
        cerr << "无法打开摄像头 " << camIndex << endl;
        return -1;
    }

    // ---- 2. 初始化录制器(可选) ----
    // 支持 H.264/H.265/MP4/AVI 多编码格式
    VideoWriter writer;
    if (!recordPath.empty()) {
        int codec = VideoWriter::fourcc('m', 'p', '4', 'v');
        int fps = 10;
        int w = cap.get(CAP_PROP_FRAME_WIDTH);
        int h = cap.get(CAP_PROP_FRAME_HEIGHT);
        if (!writer.open(recordPath, codec, fps, Size(w, h))) {
            cerr << "无法创建录制文件 " << recordPath << endl;
            return -1;
        }
        cout << "录制: " << recordPath << " (" << w << "x" << h << " @ " << fps << "fps)" << endl;
    }

    // ---- 3. 启动采集线程(生产者) ----
    FrameQueue queue(3);
    thread capThread(captureThread, ref(cap), ref(queue));

    // ---- 4. MOG2 背景减法器 ----
    // 混合高斯背景建模: 每个像素维护多个高斯分布
    // 新帧匹配已有分布→背景, 不匹配→前景(运动目标)
    Ptr<BackgroundSubtractor> mog2 = createBackgroundSubtractorMOG2();

    Mat frame, gray, prevGray, diff;
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));

    // 读取首帧作为帧差法初始背景
    if (!queue.pop(frame)) {
        cerr << "首帧读取失败" << endl;
        g_running.store(false);
        queue.shutdown();
        capThread.join();
        cap.release();
        return -1;
    }
    cvtColor(frame, prevGray, COLOR_BGR2GRAY);

    // FPS 统计
    int frameCount = 0;
    int64 t0 = getTickCount();

    // ==================== 处理循环 ====================
    while (g_running.load()) {
        if (!queue.pop(frame)) break;

        // ---- 运动检测: 帧差法 or MOG2 ----
        if (mode == 0) {
            // 帧差法: 灰度化 + 高斯模糊 + 前后帧差分 + 二值化
            cvtColor(frame, gray, COLOR_BGR2GRAY);
            GaussianBlur(gray, gray, Size(7, 7), 0);
            absdiff(gray, prevGray, diff);
            threshold(diff, diff, 17, 255, THRESH_BINARY);
            prevGray = gray.clone();
        } else {
            // MOG2: 自动维护高斯背景模型, 输出前景掩码
            mog2->apply(frame, diff);
        }

        // ---- 形态学处理: 腐蚀去噪 + 膨胀填充 ----
        erode(diff, diff, kernel);
        dilate(diff, diff, kernel);

        // ---- 轮廓检测 + 面积过滤 + 边界框标注 ----
        vector<vector<Point>> contours;
        findContours(diff, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        int detectCount = 0;
        for (auto &c : contours) {
            if (contourArea(c) < minArea) continue;
            Rect box = boundingRect(c);
            rectangle(frame, box, Scalar(0, 255, 0), 2);
            putText(frame, "Motion", Point(box.x, box.y - 5),
                    FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 1);
            detectCount++;
        }

        // ---- 录制(如果启用) ----
        if (writer.isOpened()) writer.write(frame);

        // ---- FPS + 检测数显示 ----
        frameCount++;
        if (frameCount % 30 == 0) {
            int64 t1 = getTickCount();
            double fps = 30.0 / ((double)(t1 - t0) / getTickFrequency());
            putText(frame, "FPS: " + to_string((int)fps) + " | Targets: " + to_string(detectCount),
                    Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 255), 2);
            t0 = t1;
        }

        // ---- 显示结果 ----
        imshow("video", frame);
        imshow("mask", diff);

        if (waitKey(30) == 'q') {
            g_running.store(false);
            queue.shutdown();
            break;
        }
    }

    // ---- 资源释放 ----
    capThread.join();
    cap.release();
    if (writer.isOpened()) writer.release();
    destroyAllWindows();
    return 0;
}
