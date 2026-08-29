/*
 * frame_pool.c - 帧内存池实现
 */
#include "common/frame_pool.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

FramePool* frame_pool_create(int count, size_t frame_size) {
    if (count <= 0 || frame_size == 0) return NULL;

    FramePool *pool = (FramePool*)calloc(1, sizeof(FramePool));
    if (!pool) return NULL;

    pool->total_count = count;
    pool->frame_size = frame_size;
    pool->free_count = count;

    // 分配帧结构体
    pool->frames = (Frame*)calloc(count, sizeof(Frame));
    if (!pool->frames) {
        free(pool);
        return NULL;
    }

    // 分配连续的数据缓冲区
    pool->data_buf = (uint8_t*)calloc(count, frame_size);
    if (!pool->data_buf) {
        free(pool->frames);
        free(pool);
        return NULL;
    }

    // 初始化每帧
    for (int i = 0; i < count; i++) {
        pool->frames[i].data = pool->data_buf + i * frame_size;
        pool->frames[i].capacity = frame_size;
        pool->frames[i].size = 0;
        pool->frames[i].ref_count = 0;
        pool->frames[i].pool_index = i;
        pool->frames[i].pool = pool;
    }

    pthread_mutex_init(&pool->mtx, NULL);
    pthread_cond_init(&pool->cond, NULL);

    return pool;
}

void frame_pool_destroy(FramePool *pool) {
    if (!pool) return;

    pthread_mutex_destroy(&pool->mtx);
    pthread_cond_destroy(&pool->cond);
    free(pool->data_buf);
    free(pool->frames);
    free(pool);
}

static Frame* find_free_frame_locked(FramePool *pool) {
    for (int i = 0; i < pool->total_count; i++) {
        if (pool->frames[i].ref_count == 0) {
            pool->frames[i].ref_count = 1;
            pool->frames[i].size = 0;
            pool->frames[i].pts = 0;
            pool->free_count--;
            return &pool->frames[i];
        }
    }
    return NULL;
}

Frame* frame_pool_alloc(FramePool *pool) {
    if (!pool) return NULL;

    pthread_mutex_lock(&pool->mtx);
    while (pool->free_count == 0) {
        pthread_cond_wait(&pool->cond, &pool->mtx);
    }
    Frame *f = find_free_frame_locked(pool);
    pthread_mutex_unlock(&pool->mtx);
    return f;
}

Frame* frame_pool_try_alloc(FramePool *pool) {
    if (!pool) return NULL;

    pthread_mutex_lock(&pool->mtx);
    Frame *f = (pool->free_count > 0) ? find_free_frame_locked(pool) : NULL;
    pthread_mutex_unlock(&pool->mtx);
    return f;
}

void frame_ref(Frame *frame) {
    if (!frame || !frame->pool) return;

    FramePool *pool = frame->pool;
    pthread_mutex_lock(&pool->mtx);
    frame->ref_count++;
    pthread_mutex_unlock(&pool->mtx);
}

void frame_unref(Frame *frame) {
    if (!frame || !frame->pool) return;

    FramePool *pool = frame->pool;
    pthread_mutex_lock(&pool->mtx);
    frame->ref_count--;
    if (frame->ref_count <= 0) {
        frame->size = 0;
        pool->free_count++;
        pthread_cond_signal(&pool->cond);
    }
    pthread_mutex_unlock(&pool->mtx);
}
