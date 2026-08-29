/*
 * main.cpp - 实时视频采集与运动检测系统
 *
 * 架构:
 *   采集线程(生产者) → FrameQueue → 主线程(消费者)
 *                                 ↓
 *                         MotionDetector (帧差法/MOG2)
 *                                 ↓
 *                         VideoRecorder (可选录制)
 *                                 ↓
 *                         imshow 显示结果
 *
 * 用法:
 *   ./motion_detect [摄像头索引] [模式] [最小面积] [录制文件]
 *     摄像头索引: 默认 0
 *     模式: 0=帧差法 1=MOG2 (默认1)
 *     最小面积: 默认 900
 *     录制文件: 不填则不录制
 */
#include <iostream>
#include <thread>
#include <string>
#include <atomic>
#include "frame_queue.h"
#include "motion_detector.h"
#include "video_recorder.h"

using namespace std;

// 全局运行标志(两个线程共享)
atomic<bool> g_running(true);

// ========== 采集线程(生产者) ==========
void captureThread(cv::VideoCapture &cap, FrameQueue &queue) {
    cv::Mat frame;
    while (g_running.load()) {
        cap >> frame;
        if (frame.empty()) {
            cerr << "[采集] 摄像头读取失败" << endl;
            g_running.store(false);
            queue.shutdown();
            break;
        }
        queue.push(frame.clone());
    }
}

// ========== 打印用法 ==========
void printUsage(const char *prog) {
    cout << "用法: " << prog << " [摄像头] [模式] [最小面积] [录制文件]" << endl;
    cout << "  摄像头: 摄像头索引 (默认 0)" << endl;
    cout << "  模式:   0=帧差法  1=MOG2 (默认 1)" << endl;
    cout << "  最小面积: 目标最小像素面积 (默认 900)" << endl;
    cout << "  录制文件: 输出视频路径, 不填则不录制" << endl;
    cout << endl;
    cout << "示例:" << endl;
    cout << "  " << prog << " 0 1 900 output.mp4   # MOG2模式+录制" << endl;
    cout << "  " << prog << " 0 0 500              # 帧差法模式" << endl;
}

// ========== 主函数 ==========
int main(int argc, char *argv[]) {
    // ---- 解析参数 ----
    if (argc > 1 && (string(argv[1]) == "-h" || string(argv[1]) == "--help")) {
        printUsage(argv[0]);
        return 0;
    }

    int camIndex    = (argc > 1) ? stoi(argv[1]) : 0;
    int mode        = (argc > 2) ? stoi(argv[2]) : 1;
    int minArea     = (argc > 3) ? stoi(argv[3]) : 900;
    string recordPath = (argc > 4) ? argv[4] : "";

    cout << "=========================================" << endl;
    cout << "  Video Motion Detection System" << endl;
    cout << "=========================================" << endl;
    cout << "  摄像头: " << camIndex << endl;
    cout << "  检测模式: " << (mode == 0 ? "帧差法 (FrameDiff)" : "MOG2 混合高斯") << endl;
    cout << "  最小面积: " << minArea << " px" << endl;
    if (!recordPath.empty())
        cout << "  录制文件: " << recordPath << endl;
    else
        cout << "  录制: 关闭" << endl;
    cout << "=========================================" << endl;
    cout << "  按 q 键退出" << endl;
    cout << "=========================================" << endl;

    // ---- 1. 打开摄像头 ----
    cv::VideoCapture cap;
    if (!cap.open(camIndex)) {
        cerr << "错误: 无法打开摄像头 " << camIndex << endl;
        return -1;
    }
    int width  = (int)cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int height = (int)cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    cout << "摄像头分辨率: " << width << "x" << height << endl;

    // ---- 2. 初始化录制器(可选) ----
    VideoRecorder recorder;
    if (!recordPath.empty()) {
        if (!recorder.open(recordPath, width, height, 15.0, CODEC_MP4V)) {
            cerr << "错误: 无法创建录制文件 " << recordPath << endl;
            return -1;
        }
        cout << "录制已启动: " << recordPath << endl;
    }

    // ---- 3. 初始化运动检测器 ----
    MotionDetector detector;
    detector.set_mode((DetectMode)mode);
    detector.set_min_area(minArea);

    // ---- 4. 启动采集线程 ----
    FrameQueue queue(3);
    thread capThread(captureThread, ref(cap), ref(queue));

    // ---- 5. 处理循环 ----
    cv::Mat frame;
    int frameCount = 0;
    int64 t0 = cv::getTickCount();
    double currentFps = 0.0;

    while (g_running.load()) {
        if (!queue.pop(frame)) break;

        // 运动检测
        DetectResult result = detector.detect(frame);
        detector.draw_results(frame, result);

        // FPS 统计(每30帧更新一次)
        frameCount++;
        if (frameCount % 30 == 0) {
            int64 t1 = cv::getTickCount();
            currentFps = 30.0 / ((double)(t1 - t0) / cv::getTickFrequency());
            t0 = t1;
        }

        // 叠加信息到画面
        char info[256];
        snprintf(info, sizeof(info),
                 "Mode: %s | FPS: %.1f | Targets: %d",
                 detector.mode_name().c_str(), currentFps, result.target_count);
        cv::putText(frame, info, cv::Point(10, 30),
                    cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 255), 2);

        // 录制
        if (recorder.is_recording()) recorder.write(frame);

        // 显示
        cv::imshow("video", frame);
        cv::imshow("mask", result.mask);

        // 按键处理
        int key = cv::waitKey(30);
        if (key == 'q' || key == 'Q') {
            g_running.store(false);
            queue.shutdown();
            break;
        }
    }

    // ---- 6. 资源释放 ----
    cout << "\n正在停止..." << endl;
    g_running.store(false);
    queue.shutdown();
    capThread.join();
    cap.release();
    recorder.release();
    cv::destroyAllWindows();

    cout << "已退出, 共处理 " << frameCount << " 帧" << endl;
    return 0;
}
