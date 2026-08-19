
//启用posix 2008 09技术规范
#define _POSIX_C_SOURCE 200809L 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define max 3
// pthread_mutex_t mut;
// pthread_spinlock_t spin;
pthread_rwlock_t rw;
int n=0;
void *rd(void *p)
{
while(1)
{
    pthread_rwlock_rdlock(&rw);
    printf("线程%d  -读取了%d\n",pthread_self(),n);
    sleep(1);
    pthread_rwlock_unlock(&rw);
     //sleep(1);
  
}
}
void *wt(void *p)
{
while(1)
{
   pthread_rwlock_wrlock(&rw);
    n++;
    printf("===线程  写入了%d\n",n);
    sleep(2);
  pthread_rwlock_unlock(&rw);
  sleep(5);
}

}
int main()
{
    pthread_rwlockattr_t attr;
    pthread_rwlockattr_init(&attr);
    pthread_rwlockattr_setkind_np(&attr, PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP);
    pthread_rwlock_init(&rw, &attr);
    pthread_t rtid[max];
    pthread_t wtid[max];
   // pthread_rwlock_init(&rw,NULL);

    for(int i=0;i<max;i++)
    {
        pthread_create(&rtid[i],NULL,rd,(void *)i);
    }
    for(int i=0;i<max;i++)
    {
        pthread_create(&wtid[i],NULL,wt,(void *)i);
    }






    for(int i=0;i<max;i++)
    {
        pthread_join(rtid[i],NULL);
    }
    for(int i=0;i<max;i++)
    {
        pthread_join(wtid[i],NULL);
    }



    pthread_rwlock_destroy(&rw);

    
    return 0;

}
