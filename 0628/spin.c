// 启用 posix 2008 09
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
pthread_spinlock_t spin;
pthread_mutex_t mut;
#define max 4
int n=0;
void *task(void *p)
{
int t =n;
    for(int i=0;i<100000;i++)
    {
        t++;
    }
    pthread_spin_lock(&spin);
    //pthread_mutex_lock(&mut);
    n+=t;
  //  pthread_mutex_unlock(&mut);
     pthread_spin_unlock(&spin);
}
int main()
{
    pthread_spin_init(&spin, 0);
    pthread_mutex_init(&mut, NULL);
    pthread_t tid[max];
    for (int i = 0; i < max; i++)
    {
        // 堆空间
        int *id = (int *)malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&tid[i], NULL, task, id);
    }

    for (int i = 0; i < max; i++)
    {
        pthread_join(tid[i], NULL);
    }
    printf("%d",n);
    pthread_spin_destroy(&spin);
    pthread_mutex_destroy(&mut);
    return 0;
}