//统计所有.c.h文件的行数
#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
//线程数
#define MAX  8 
#define MAX_ENQUEUE 80000
pthread_mutex_t mut;
pthread_rwlock_t rw;
pthread_spinlock_t spin;
pthread_cond_t full;
pthread_cond_t empty;
const char *root_path = "/home/xuezha/linux-7.0.10";

long long total_lines = 0;//总行数
int dir_count = 0; //目录数
int file_count = 0;//文件数

char **file_paths; // 存储文件路径的指针数组
int c_h_count = 0; //.C.H 文件统计

typedef struct 
 {
    char *file[MAX_ENQUEUE];//文件路径
    int front;//队头
    int rear;//队尾
    int count;//当前队列元素
 } enqueue,taskq;
 void enqueu(taskq *q, char *file)
{
    if(q->count==MAX_ENQUEUE)
    {
        printf("队列已满\n");
        return;
    }
    q->file[q->rear] = file;//入队
    q->rear=(q->rear+1)%MAX_ENQUEUE;//入队
    q->count++;//队列元素加一
    
}
char *dequeu(taskq *q)
{
    if(q->count==0)
    {
        printf("队列已空\n");
        return     NULL;
    }
    q->file[q->front]=NULL;//出队
    q->front=(q->front+1)%MAX_ENQUEUE;//出队
    return q->file[q->front];
    
    q->count--;//队列元素减一
    
}
void traverse_dir(const char *name, taskq *q)
{
    DIR *dir = opendir(name);
    if (!dir)
    {
        perror("无法打开目录");
        return;
    }

    struct dirent *entry;
    char path[10000];

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] == '.')
        {
            continue;
        }

        sprintf(path, "%s/%s", name, entry->d_name);

        if (entry->d_type == DT_DIR)
        {
            dir_count++; // 目录数加一
            // print_indent(depth);
            // printf("[%s]\n", entry->d_name);
            traverse_dir(path, q);
        }
        else if (entry->d_type == DT_REG)
        {
            file_count++;
            // print_indent(depth);
            // printf("%s\n", entry->d_name);

            // 获取.c .h文件的路径
            char *ext = strrchr(entry->d_name, '.'); // 获取文件扩展名
            if (ext != NULL)
            { // 确保有扩展名
                // 检查是否是 .c 或 .h 文件
                if (strcmp(ext, ".c") == 0 || strcmp(ext, ".h") == 0)
                {
                    c_h_count++;
                    enqueu(q,strdup(path));
                }
            }
        }
    }

    closedir(dir);
}
void *producter(void *p)
{
    taskq *q = (taskq *)p;
    pthread_mutex_lock(&mut);
    if(q->count==MAX_ENQUEUE)
    {
        printf("队列已满\n");
       pthread_cond_wait(&full,&mut);
       pthread_cond_signal(&empty);
    pthread_cond_broadcast(&empty);
        return NULL;
    }
    traverse_dir(q->file[q->front ], q);
    
   pthread_mutex_unlock(&mut);
}
void *consumer(void *p)
{
    taskq *q = (taskq *)p;
    pthread_mutex_lock(&mut);
    if(q->count==0)
    {
        pthread_cond_wait(&empty,&mut);
       pthread_cond_signal(&full);
       pthread_cond_broadcast(&full);
       return NULL;
    }
    pthread_rwlock_wrlock(&rw);
     char *file = dequeu(q);
       FILE *fp = fopen(file, "r");
       if (!fp)
       {
           printf("无法打开文件");
           return NULL;
       }
      if()
       fclose(fp);
    pthread_rwlock_unlock(&rw);
}

// 线程参数结构体
// typedef struct
// {
//     int start;     // 起始索引
//     int end;       // 结束索引
//     long long sum; // 该线程统计的行数
// } ThreadArg;

void print_indent(int depth)
{
    for (int i = 0; i < depth; i++)
    {
        printf("  ");
    }
}

// void count_lines(const char *filepath)
// {
//     FILE *fp = fopen(filepath, "r");
//     if (!fp)
//         return;

//     char ch;
//     while ((ch = fgetc(fp)) != EOF)
//     {
//         if (ch == '\n')
//         {
//             total_lines++;
//         }
//     }
//     fclose(fp);
// }

// 线程函数 - 统计指定范围的文件行数
// void *worker_thread(void *arg)
// {
  
     
//     return NULL;
// }



int main()
{
    // 初始化互斥锁
    pthread_mutex_init(&mut, NULL);
    // 初始化读写锁
    pthread_rwlock_init(&rw, NULL);
    // 初始化自旋锁
    pthread_spin_init(&spin, 0);
    pthread_cond_init(&full, NULL);
    pthread_cond_init(&empty, NULL);

    taskq q={0};
    q.front=0;
    q.rear=0;
    q.count=0;


    // 遍历目录收集文件
  
    // printf("目录树:\n");
    // printf("[%s]\n", root_path);
   

    const int THREAD_NUM = MAX;     // 使用m个总线程
    pthread_t pro[THREAD_NUM/2]; // 生产者 线程池
    pthread_t con[THREAD_NUM/2+1]; // 消费者 线程池
  
    for (int i = 0; i < THREAD_NUM/2; i++)
    {
        pthread_create(&pro[i], NULL, producter, &q);
    }
     for (int i = 0; i < THREAD_NUM/2; i++)
    {
        pthread_create(&con[i], NULL, consumer, &q);
    }
    
    // 等待所有线程完成
    long long total_c_h_lines = 0; //.c .h总行数
    for (int i = 0; i < THREAD_NUM/2; i++)
    {
        pthread_join(pro[i], NULL); // 等待线程结束 非分离态用join
    }
    for (int i = 0; i < THREAD_NUM/2+1; i++)
    {
        pthread_join(con[i], NULL); // 等待线程结束 非分离态用join
    }

    // 输出结果
    printf("统计结果:\n");
    printf("文件夹数目: %d\n", dir_count);
    printf("文件数目: %d\n", file_count);
    printf(".c/.h 文件数目: %d\n", c_h_count);
    printf(".c/.h 文件总行数: %lld\n", total_c_h_lines);

    // 释放内存
    for (int i = 0; i < c_h_count; i++)
    {
        free(file_paths[i]);
    }
    free(file_paths);

    // 销毁互斥锁
    pthread_mutex_destroy(&mut);
    // 销毁读写锁
    pthread_rwlock_destroy(&rw);
    // 销毁自旋锁
    pthread_spin_destroy(&spin);

    return EXIT_SUCCESS;
}