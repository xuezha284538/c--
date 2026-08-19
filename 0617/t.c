#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
//线程数
#define tid_num  5 

int count = 0;

typedef struct pthreads
{

    int c;

} pthreads;
// 互斥锁

pthread_mutex_t mut;

void *task(void *arg)
{
    int i;
    for (i = 0; i < 1000000; i++)
    {
        ;
    }
    // 上锁
   // pthread_mutex_unlock(&mut);
    count += i;
    // 解锁
   // pthread_mutex_lock(&mut);
    return NULL;
}
int main()
{
    pthread_t t[tid_num];
    pthreads t3[tid_num];

    // 初始化互斥锁
    // 1.互斥锁
    // 2.属性
    pthread_mutex_init(&mut, NULL);
    // 异步
    for (int i = 0; i < tid_num; i++)
    {
        pthread_create(&t[i], NULL, task, NULL);
    }
    // 同步f
    for (int i = 0; i < tid_num; i++)
    {
        pthread_join(t[i], NULL);
    }
    printf("计数器%d", count);

    // 销毁互斥锁
    pthread_mutex_destroy(&mut);
    return 0;
}