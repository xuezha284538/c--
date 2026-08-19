#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
// 信号量

#include <semaphore.h>
// 信号量
sem_t sem;

int n=0;
void * task(void *)
{
    for(int i=0;i<100000;i++)
    {
        //信号量-1  二元 到零阻塞
        sem_wait(&sem);
        n++;

        //信号量+1  
        sem_post(&sem);
    }
}


int main()
{
    //1.信号量本身
    //2.类型 1是进程间 0是线程间通信  0进程之间不共享 1进程间共享
    //3.信号量的值 1 二元信号量;  其他值  计数器信号量
    sem_init(&sem,0,1);
    pthread_t t1;
    pthread_t t2;

    pthread_create(&t1,NULL,task,NULL);
    pthread_create(&t2,NULL,task,NULL);
    
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);



    printf("%d\n",n);
    //销毁
    sem_destroy(&sem);
    return 0;
}