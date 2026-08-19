#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define max 5
int n=0;
pthread_mutex_t mut;
// 条件变量r
pthread_cond_t  cond_full;//
pthread_cond_t  cond_empty;
void * producter(void *p)
{
    for(int i=0;i<10;i++)
    {
        pthread_mutex_lock(&mut);
        while(n==max)
        {
            printf("满了\n");
            pthread_cond_wait(&cond_empty,&mut);
        }
        
        n++;
        printf("生产了%d\n",n);
        //唤醒消费者
        pthread_cond_signal(&cond_full);
        pthread_cond_broadcast(&cond_full);
        pthread_mutex_unlock(&mut);
    }

}



void * consumer(void *p)
{
  for(int i=0;i<10;i++)
    {
        pthread_mutex_lock(&mut);
       while(n==0)
        {
            printf("队列为空\n");
            //等待生产者发生cond_full
            pthread_cond_wait(&cond_full,&mut);
        }
        printf("消费了%d\n",n);
        n--;
         //发一个消息唤醒等待——empty的线程/ 唤醒正在等待的线程(只能唤醒一个)
        pthread_cond_signal(&cond_empty);
        pthread_cond_broadcast(&cond_empty);
        pthread_mutex_unlock(&mut);

    }


}

int main()
{
    pthread_t con;
    pthread_t pro;
    pthread_mutex_init(&mut, NULL);

    pthread_cond_init(&cond_empty, NULL);
    pthread_cond_init(&cond_full, NULL);

    pthread_create(&con,NULL,consumer,NULL);
    pthread_create(&pro,NULL,producter,NULL);

    pthread_join(&pro,NULL);
    pthread_join(&con,NULL);
// 销毁
    pthread_mutex_destroy(&mut);
    pthread_cond_destroy(&cond_empty);
    pthread_cond_destroy(&cond_full);

    return 0 ;
}