/*
 * main.c - 视频广播发送端主程序 (框架)
 *
 * 启动流程:
 *   1. 初始化视频/音频采集
 *   2. 初始化视频/音频编码器
 *   3. 初始化 RTP 打包器
 *   4. 初始化 UDP 组播发送
 *   5. 启动 TCP 信令服务
 *   6. 采集线程 → 编码线程 → 发送线程 (通过无锁队列/内存池传递数据)
 *
 * 目前是框架代码, 各模块接口已定义, 具体实现待完善
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#include "capture/video_capture.h"
#include "capture/audio_capture.h"
#include "codec/video_encoder.h"
#include "codec/audio_encoder.h"
#include "transport/rtp_packetizer.h"
#include "transport/multicast_sender.h"
#include "signaling/signaling_server.h"
#include "common/lock_free_queue.h"
#include "common/frame_pool.h"

static int g_running = 1;

void sig_handler(int sig) {
    (void)sig;
    g_running = 0;
    printf("\n正在停止...\n");
}

static void print_usage(const char *prog) {
    printf("用法: %s [选项]\n", prog);
    printf("  -d <设备>    视频设备 (默认 /dev/video0)\n");
    printf("  -W <宽度>    视频宽度 (默认 640)\n");
    printf("  -H <高度>    视频高度 (默认 480)\n");
    printf("  -f <帧率>    视频帧率 (默认 30)\n");
    printf("  -a <设备>    音频设备 (默认 default)\n");
    printf("  -m <地址>    组播地址 (默认 239.255.0.1)\n");
    printf("  -p <端口>    媒体端口 (默认 5004)\n");
    printf("  -s <端口>    信令端口 (默认 8000)\n");
    printf("  -h           显示帮助\n");
}

// 信令回调: 客户端加入
static void on_client_join(int fd, const char *ip, int port, void *user_data) {
    (void)user_data;
    printf("[信令] 客户端加入: %s:%d (fd=%d)\n", ip, port, fd);
}

// 信令回调: 客户端离开
static void on_client_leave(int fd, const char *ip, int port, void *user_data) {
    (void)user_data;
    (void)ip;
    (void)port;
    printf("[信令] 客户端离开: fd=%d\n", fd);
}

// 信令回调: 收到消息
static void on_message(int fd, const SigMessage *msg, void *user_data) {
    (void)user_data;
    printf("[信令] 收到消息: fd=%d, type=%u, len=%u\n", fd, msg->type, msg->length);
}

int main(int argc, char *argv[]) {
    // 默认参数
    const char *video_dev = "/dev/video0";
    int width = 640, height = 480, fps = 30;
    const char *audio_dev = "default";
    const char *mcast_addr = "239.255.0.1";
    int media_port = 5004;
    int sig_port = 8000;

    // 解析参数
    int opt;
    while ((opt = getopt(argc, argv, "d:W:H:f:a:m:p:s:h")) != -1) {
        switch (opt) {
            case 'd': video_dev = optarg; break;
            case 'W': width = atoi(optarg); break;
            case 'H': height = atoi(optarg); break;
            case 'f': fps = atoi(optarg); break;
            case 'a': audio_dev = optarg; break;
            case 'm': mcast_addr = optarg; break;
            case 'p': media_port = atoi(optarg); break;
            case 's': sig_port = atoi(optarg); break;
            case 'h': print_usage(argv[0]); return 0;
            default:  print_usage(argv[0]); return 1;
        }
    }

    printf("========================================\n");
    printf(" Video Broadcast Server\n");
    printf("========================================\n");
    printf(" 视频: %s (%dx%d @ %dfps)\n", video_dev, width, height, fps);
    printf(" 音频: %s\n", audio_dev);
    printf(" 组播: %s:%d\n", mcast_addr, media_port);
    printf(" 信令: 端口 %d\n", sig_port);
    printf("========================================\n");

    signal(SIGINT, sig_handler);
    signal(SIGPIPE, SIG_IGN);

    // ===== 1. 视频采集 =====
    VideoCaptureConfig vcfg = {0};
    vcfg.device = video_dev;
    vcfg.width = width;
    vcfg.height = height;
    vcfg.fps = fps;
    VideoCapture *vcap = video_capture_open(&vcfg);
    if (!vcap) {
        fprintf(stderr, "错误: 无法打开视频设备 %s\n", video_dev);
        return 1;
    }
    printf("[采集] 视频采集已启动\n");

    // ===== 2. 视频编码器 =====
    VideoEncoderConfig venc_cfg = {0};
    venc_cfg.width = width;
    venc_cfg.height = height;
    venc_cfg.fps = fps;
    venc_cfg.bitrate = 2000000; // 2Mbps
    venc_cfg.gop_size = fps * 2; // 2秒一个关键帧
    venc_cfg.preset = "ultrafast";
    VideoEncoder *venc = video_encoder_create(&venc_cfg);
    if (!venc) {
        fprintf(stderr, "警告: 视频编码器初始化失败 (将跳过编码)\n");
    }

    // ===== 3. RTP 打包器 =====
    RtpPacketizer video_rtp;
    rtp_packetizer_init(&video_rtp, RTP_PT_H264, 0x12345678, 90000);

    // ===== 4. UDP 组播发送 =====
    MulticastSenderConfig mcfg = {0};
    mcfg.multicast_addr = mcast_addr;
    mcfg.port = media_port;
    mcfg.ttl = 64;
    MulticastSender *sender = multicast_sender_create(&mcfg);
    if (!sender) {
        fprintf(stderr, "错误: 无法创建组播发送器\n");
        video_capture_close(vcap);
        return 1;
    }
    printf("[传输] UDP组播发送已启动: %s:%d\n", mcast_addr, media_port);

    // ===== 5. 信令服务 =====
    SignalingServer *sig_srv = sig_server_create(sig_port);
    if (sig_srv) {
        sig_server_set_callbacks(sig_srv, on_client_join, on_client_leave,
                                  on_message, NULL);
        sig_server_run(sig_srv);
        printf("[信令] TCP信令服务已启动: 端口 %d\n", sig_port);
    }

    // ===== 6. 主循环 (采集 → 编码 → 打包 → 发送) =====
    // TODO: 多线程架构: 采集线程 + 编码线程 + 发送线程
    // 目前单线程占位演示
    uint8_t *frame_buf = (uint8_t*)malloc(width * height * 2);
    uint8_t *enc_buf = (uint8_t*)malloc(width * height);
    uint8_t *rtp_buf = (uint8_t*)malloc(1500);
    size_t enc_size, rtp_size;
    int64_t pts;
    int frame_count = 0;

    printf("\n开始广播, 按 Ctrl+C 停止...\n\n");

    while (g_running) {
        // 采集
        size_t fsize = width * height * 2;
        if (video_capture_get_frame(vcap, frame_buf, &fsize, &pts) < 0) {
            fprintf(stderr, "错误: 视频采集失败\n");
            break;
        }

        // 编码 (如果编码器可用)
        if (venc) {
            enc_size = width * height;
            int ret = video_encoder_encode(venc, frame_buf, fsize, pts,
                                            enc_buf, &enc_size, &pts);
            if (ret < 0) continue;
        } else {
            // 无编码器, 直接用原始数据占位
            memcpy(enc_buf, frame_buf, fsize);
            enc_size = fsize;
        }

        // RTP 打包
        size_t pkt_sizes[16];
        int pkt_count = rtp_packetizer_packetize(&video_rtp, enc_buf, enc_size,
                                                  pts, rtp_buf, 16, pkt_sizes);
        if (pkt_count <= 0) continue;

        // 发送
        // TODO: 多个包循环发送
        multicast_sender_send(sender, rtp_buf, pkt_sizes[0]);

        frame_count++;
        if (frame_count % (fps * 5) == 0) {
            printf("已发送 %d 帧...\n", frame_count);
        }

        // 简单帧率控制 (实际应该用采集时钟)
        usleep(1000000 / fps);
    }

    // ===== 清理 =====
    printf("\n正在清理资源...\n");

    if (sig_srv) {
        sig_server_stop(sig_srv);
        sig_server_destroy(sig_srv);
    }
    if (sender) multicast_sender_destroy(sender);
    if (venc)   video_encoder_destroy(venc);
    if (vcap)   video_capture_close(vcap);

    free(frame_buf);
    free(enc_buf);
    free(rtp_buf);

    printf("已退出, 共发送 %d 帧\n", frame_count);
    return 0;
}
