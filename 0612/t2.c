// t2.cc
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
void *task(void *)
{
    int *val = malloc(sizeof(int));

    pthread_t tid = pthread_self();
    for (int i = 0; i < 9; i++)
    {
        printf("%ld, %d\n", tid, i);
        if (i == 5)
        {
            *val = 5;
            pthread_exit((void *)val); // 退出当前线程 之后的代码不执行
        }
    }
    printf("\n");
    *val = 9;
    return (void *)val;
}
int main()
{
    pthread_t tid;
    if (pthread_create(&tid, NULL, task, NULL) != 0)
    {
        perror("pthread_create error");
        return 1;
    }
    // sleep(2);
    void *val;
    pthread_join(tid, &val);
    int v = *((int *)val);
    printf("子线程返回的参数:%d\n", v);

    // 退出主线程
    pthread_exit(NULL); // 之后的代码不执行
    // pthread_cancel(); 取消指定id的线程
    printf("主线程ID: %ld\n", pthread_self());
    printf("sub:%ld\n", tid);
    free(val);
    return 0;
}