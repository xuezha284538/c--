// t1.c
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct par
{
    int start; // 输入参数
    int end;   // 输入参数
    int c;     // 输出参数
} Par, par;

/**
 * 判断参数是不是质数
 *
 *
 */
bool is_prime(int n)
{

    bool flag = true;
    // 2~n-1
    for (int i = 2; i * i <= n; i++)
    {
        if (n % i == 0)
        {
            flag = false;
            break;
        }
    }
    return flag;
}
void *task1(void *arg)
{
    // int *p=(int * )arg;
    int p = *((int *)arg);
    // pthread_self(); 获取当前线程id
    // 栈空间 局部变量 函数结束即释放
    int c = 0;
    for (int i = 2; i < p; i++)
    {
        if (is_prime(i))
        {
            printf("%d,", i);
            c++;
        }
    }
    // sleep(1);
    // 动态内存分配
    // 避免返回局部变量的地址 出现野指针问题
    int *r = malloc(sizeof(int));
    *r = c;
    printf("\n");
    return (void *)r;
}

void *task2(void *arg)
{
    par *p = (par *)arg;

    for (int i = p->start; i < p->end; i++)
    {

        if (is_prime(i))
        {
            printf("%d,", i);
            p->c++;
        }
    }
    printf("\n");
    return NULL;
}

void *task3(void *)

{
    for (;;)
    {
        printf("%ld欢呼...\n", pthread_self());
    }
    // sleep(1);
}

int main()
{

    pthread_t t1;
    pthread_t t2;
    pthread_t t3;

    // Par p={1000,2000};
    Par *p = malloc(sizeof(Par));
    p->start = 1000;
    p->end = 1050;
    p->c = 0;
    pthread_create(&t2, NULL, task2, p);
    pthread_join(t2, NULL);

    printf("总数:%d\n", p->c);

    // pthread_create(&t3, NULL, task3, NULL);
    int n = 50;
    pthread_create(&t1, NULL, task1, &n);

    // 模板编程 泛型
    void *r;
    // 第二个参数 void**  二级指针
    pthread_join(t1, &r);
    int result = *((int *)r);
    free(r);
    printf("返回值:%d\n", result);

    free(p);
    // pthread_join(t3, NULL);

    return 0;
}