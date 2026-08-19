#include <stdio.h>
#include <stdlib.h>
// 线程库
#include <pthread.h>
#include <unistd.h>
// 信号量
#include <semaphore.h>
sem_t sem;
// 线程最大总数
#define MAX 8
// 目前可用数量
#define N 2
void *task(void *p)
{
    int id = *((int *)p);
    free(p);
    sem_wait(&sem);//获得资源 sem>0 -- sem=0 阻塞  

    printf("线程%d启动\n", id);



    sleep(1);



    printf("线程%d结束~~~\n", id);
    //sem++ 释放资源 
    sem_post(&sem);


}
int main()
{
    sem_init(&sem, 0, N);
    pthread_t t[MAX];
    for (int i = 0; i < MAX; i++)
    {
        // 堆空间
        int *id = (int *)malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&t[i], NULL, task, id);
    }

    for (int i = 0; i < MAX; i++)
    {
        pthread_join(t[i], NULL);
    }

    sem_destroy(&sem);
    return 0;
}