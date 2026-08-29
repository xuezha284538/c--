/*
 * lock_free_queue.c - 无锁环形队列实现 (C11 原子)
 *
 * 单生产者-单消费者无锁环形字节队列
 * 使用 memory_order_release/acquire 保证可见性
 */
#include "common/lock_free_queue.h"
#include <stdint.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

LockFreeQueue* lfq_create(size_t capacity) {
    if (capacity == 0) return NULL;

    LockFreeQueue *q = (LockFreeQueue*)calloc(1, sizeof(LockFreeQueue));
    if (!q) return NULL;

    q->capacity = capacity + 1; // 多留一个空位区分满和空
    q->buffer = (uint8_t*)malloc(q->capacity);
    if (!q->buffer) {
        free(q);
        return NULL;
    }

    atomic_store_explicit(&q->write_idx, 0, memory_order_relaxed);
    atomic_store_explicit(&q->read_idx, 0, memory_order_relaxed);
    return q;
}

void lfq_destroy(LockFreeQueue *q) {
    if (!q) return;
    free(q->buffer);
    free(q);
}

static size_t next_idx(const LockFreeQueue *q, size_t idx) {
    return (idx + 1) % q->capacity;
}

int lfq_push(LockFreeQueue *q, const void *data, size_t len) {
    if (!q || !data || len == 0) return -1;

    size_t w = atomic_load_explicit(&q->write_idx, memory_order_relaxed);
    size_t r = atomic_load_explicit(&q->read_idx, memory_order_acquire);

    // 计算可用空间
    size_t free_space = (r > w) ? (r - w - 1) : (q->capacity - w + r - 1);
    if (len > free_space) return -1; // 队列满

    // 写入 (处理环形回绕)
    size_t first_chunk = MIN(len, q->capacity - w);
    memcpy(q->buffer + w, data, first_chunk);
    if (len > first_chunk) {
        memcpy(q->buffer, (const uint8_t*)data + first_chunk, len - first_chunk);
    }

    // 更新写指针, release 保证数据对消费者可见
    atomic_store_explicit(&q->write_idx,
                          (w + len) % q->capacity,
                          memory_order_release);
    return 0;
}

size_t lfq_pop(LockFreeQueue *q, void *data, size_t max_len) {
    if (!q || !data || max_len == 0) return 0;

    size_t r = atomic_load_explicit(&q->read_idx, memory_order_relaxed);
    size_t w = atomic_load_explicit(&q->write_idx, memory_order_acquire);

    if (r == w) return 0; // 队列空

    // 计算可读数据量
    size_t avail = (w > r) ? (w - r) : (q->capacity - r + w);
    size_t to_read = MIN(avail, max_len);

    // 读取 (处理环形回绕)
    size_t first_chunk = MIN(to_read, q->capacity - r);
    memcpy(data, q->buffer + r, first_chunk);
    if (to_read > first_chunk) {
        memcpy((uint8_t*)data + first_chunk, q->buffer, to_read - first_chunk);
    }

    // 更新读指针
    atomic_store_explicit(&q->read_idx,
                          (r + to_read) % q->capacity,
                          memory_order_release);
    return to_read;
}

int lfq_empty(const LockFreeQueue *q) {
    if (!q) return 1;
    size_t r = atomic_load_explicit(&q->read_idx, memory_order_acquire);
    size_t w = atomic_load_explicit(&q->write_idx, memory_order_acquire);
    return r == w;
}

int lfq_full(const LockFreeQueue *q) {
    if (!q) return 1;
    size_t w = atomic_load_explicit(&q->write_idx, memory_order_acquire);
    size_t r = atomic_load_explicit(&q->read_idx, memory_order_acquire);
    return (w + 1) % q->capacity == r;
}

size_t lfq_size(const LockFreeQueue *q) {
    if (!q) return 0;
    size_t w = atomic_load_explicit(&q->write_idx, memory_order_acquire);
    size_t r = atomic_load_explicit(&q->read_idx, memory_order_acquire);
    return (w >= r) ? (w - r) : (q->capacity - r + w);
}

size_t lfq_capacity(const LockFreeQueue *q) {
    return q ? q->capacity : 0;
}
