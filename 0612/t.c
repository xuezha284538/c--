// t.c
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
pthread_t tid;
void *task(void *)
{
    for (int i = 0; i < 20; i++)
    {
        printf("--------------子线程%d\n", i);
        sleep(1);
    }
}
int main()
{
    // 线程id 标识 long int
    // pthread_attr
    // 创建线程
    // 1.线程编号 id
    // 2.属性
    // 3.执行,函数指针,函数
    // 4.函数参数
    int r = pthread_create(&tid, NULL, task, NULL);
    // 1.线程编号
    // 2.线程返回值

    printf("是否成功:%d\n", r);

    printf("id:%ld\n", tid);
    // printf("主线程\n");

    for (int i = 0; i < 9; i++)
    {
        printf("main线程%d\n", i);
        sleep(1);
        pthread_join(tid, NULL);
    }

    return 0;
}