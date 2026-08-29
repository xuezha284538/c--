/*
 * frame_pool.h - 帧内存池
 *
 * 预分配帧缓冲区, 使用引用计数管理生命周期
 * 避免实时 malloc/free 的性能波动和内存碎片
 */
#ifndef FRAME_POOL_H
#define FRAME_POOL_H

#include <stdint.h>
#include <stddef.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

// 帧类型
typedef enum {
    FRAME_VIDEO = 0,
    FRAME_AUDIO = 1
} FrameType;

// 帧结构
typedef struct Frame {
    FrameType type;
    uint8_t *data;      // 数据指针 (指向内存池内的缓冲区)
    size_t size;        // 实际数据大小
    size_t capacity;    // 缓冲区总容量
    int64_t pts;        // 显示时间戳 (微秒)
    int ref_count;      // 引用计数
    int pool_index;     // 在池中的索引 (内部使用)
    struct FramePool *pool; // 所属内存池
} Frame;

// 内存池
typedef struct FramePool {
    Frame *frames;      // 帧数组
    uint8_t *data_buf;  // 连续数据缓冲区
    int total_count;    // 总帧数
    size_t frame_size;  // 每帧数据大小
    int free_count;     // 空闲帧数
    pthread_mutex_t mtx;
    pthread_cond_t cond;
} FramePool;

// 创建内存池
// count: 帧数量, frame_size: 每帧数据大小
FramePool* frame_pool_create(int count, size_t frame_size);

// 销毁内存池
void frame_pool_destroy(FramePool *pool);

// 申请一帧 (阻塞, 直到有可用帧)
Frame* frame_pool_alloc(FramePool *pool);

// 申请一帧 (非阻塞, 无空闲帧返回 NULL)
Frame* frame_pool_try_alloc(FramePool *pool);

// 增加引用计数
void frame_ref(Frame *frame);

// 减少引用计数, 减到0归还到池
void frame_unref(Frame *frame);

#ifdef __cplusplus
}
#endif

#endif // FRAME_POOL_H
