/*
 * lock_free_queue.h - 无锁环形队列 (单生产者-单消费者)
 *
 * 使用 C11 原子变量实现无锁, 避免 mutex 系统调用开销
 * 适用于采集线程 -> 编码线程 等高吞吐低延迟场景
 */
#ifndef LOCK_FREE_QUEUE_H
#define LOCK_FREE_QUEUE_H

#include <stdatomic.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

// 字节队列 (存储原始数据)
typedef struct {
    size_t capacity;
    uint8_t *buffer;

    // 读写指针用原子变量
    atomic_size_t write_idx;
    atomic_size_t read_idx;
} LockFreeQueue;

// 创建队列 (字节队列)
LockFreeQueue* lfq_create(size_t capacity);

// 销毁
void lfq_destroy(LockFreeQueue *q);

// 入队
// 返回 0 成功, -1 队列满
int lfq_push(LockFreeQueue *q, const void *data, size_t len);

// 出队
// 返回读取的字节数, 0 表示空
size_t lfq_pop(LockFreeQueue *q, void *data, size_t max_len);

// 是否为空
int lfq_empty(const LockFreeQueue *q);

// 是否满
int lfq_full(const LockFreeQueue *q);

// 当前数据量
size_t lfq_size(const LockFreeQueue *q);

// 容量
size_t lfq_capacity(const LockFreeQueue *q);

#ifdef __cplusplus
}
#endif

#endif // LOCK_FREE_QUEUE_H
