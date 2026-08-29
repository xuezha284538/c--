# Video Broadcast Linux

单向视频广播系统 —— 基于 Linux 的一对多低延迟音视频广播方案。

## 技术栈

C++、Qt6、FFmpeg、V4L2、ALSA、UDP组播、RTP

## 架构

五层分层架构 + "UDP组播+RTP媒体流+TCP独立信令"双通路

### 目录结构

```
video-broadcast/
├── include/
│   ├── capture/           # 采集层 (V4L2 + ALSA)
│   ├── codec/             # 编码层 (FFmpeg H.264 + AAC)
│   ├── transport/         # 传输层 (UDP组播 + RTP)
│   ├── signaling/         # 信令层 (TCP)
│   └── common/            # 公共组件 (无锁队列/内存池/音视频同步)
├── src/
├── CMakeLists.txt
└── README.md
```

## 核心优化

- 无锁环形队列: 采集与编码线程间无锁通信
- 帧内存池: 预分配帧内存,避免频繁分配释放
- 音视频同步: 基于 PTS 的同步算法
- 端到端延迟 ≤ 200ms,支持 100+ 客户端
