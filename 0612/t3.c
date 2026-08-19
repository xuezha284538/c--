#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
void *task(void *p)
{
    int n = *((int *)p);
    pthread_t tid;
    tid = pthread_self();
    for (int i = 1; i <= n; i++)
    {
        printf("%d %ld\n", i, tid);
    }
    return NULL;
}
int main()
{
    pthread_t tid;
    // 方案一---------------------------------------- 可以和主线程分离
    // 线程id
    // 线程属性
    pthread_attr_t *attr;
    pthread_attr_init(attr);
    // 分离态子线程 后台执行 不返回数据 不join  异步执行 资源管理模式 是自动释放
    pthread_attr_setdetachstate(attr, PTHREAD_CREATE_DETACHED);
    //
    int n = 9;
    if (pthread_create(&tid, attr, task, &n) != 0)
    {
        perror("error\n");
        return 1;
    }

    pthread_attr_destroy(attr);
    printf("sub :%ld\n", tid);
    printf("main:%ld\n", pthread_self());
    pthread_exit(NULL);
    // exit的以下的代码不可到达

    // 方案二---------------------------------------- 分离态 不可join
    //  pthread_create(&tid,NULL,task,NULL); //第二个参数不可改变
    //  pthread_detach(tid);

    return 0;
}