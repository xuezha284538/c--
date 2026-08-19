//统计所有.c.h文件的行数
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>

int dir_count = 0;
int file_count = 0;
//long long total_lines = 0;
char **file_paths; // 存储文件路径的指针数组
int c_h_count = 0; //.C.H 文件统计

// 线程参数结构体
typedef struct
{
    int start;     // 起始索引
    int end;       // 结束索引
    long long sum; // 该线程统计的行数
} ThreadArg;

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
void *worker_thread(void *arg)
{
    ThreadArg *targ = (ThreadArg *)arg; // 转换参数类型
    targ->sum = 0;                      // 初始化结果

    // 遍历分配给自己的文件范围
    for (int i = targ->start; i < targ->end; i++)
    {
        FILE *fp = fopen(file_paths[i], "r");
        if (fp)
        {
            char ch;
            while ((ch = fgetc(fp)) != EOF)
            {
                if (ch == '\n')
                {
                    targ->sum++; // 统计行数
                }
            }
            fclose(fp);
        }
    }
    return NULL;
}

void traverse_dir(const char *name, int depth)
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
            traverse_dir(path, depth + 1);
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
                    // 分配内存存储路径
                    file_paths[c_h_count] = malloc(strlen(path) + 1);
                    // 复制路径到数组
                    strcpy(file_paths[c_h_count], path);
                    // 计数器加1
                    c_h_count++;
                }
            }
        }
    }

    closedir(dir);
}

int main()
{
    // 初始化文件路径数组
    file_paths = malloc(100000 * sizeof(char *)); //任务队列
    c_h_count = 0;

    // 遍历目录收集文件
    const char *root_path = "/home/xuezha/linux-7.0.10";
    // printf("目录树:\n");
    // printf("[%s]\n", root_path);
    traverse_dir(root_path, 1);

    const int THREAD_NUM = 4;     // 使用m个线程
    pthread_t threads[THREAD_NUM]; // 线程ID的数组 线程池
    ThreadArg args[THREAD_NUM];    // 线程参数的数组 线程

    // 分配任务给每个线程
    int files_per_thread = c_h_count / THREAD_NUM;
    for (int i = 0; i < THREAD_NUM; i++)
    {
        args[i].start = i * files_per_thread;
        // 最后一个线程处理剩余所有文件
        args[i].end = (i == THREAD_NUM - 1) ? c_h_count : (i + 1) * files_per_thread;
        // 创建线程
        pthread_create(&threads[i], NULL, worker_thread, &args[i]);
    }

    // 等待所有线程完成并汇总结果
    long long total_c_h_lines = 0; //.c .h总行数
    for (int i = 0; i < THREAD_NUM; i++)
    {
        pthread_join(threads[i], NULL); // 等待线程结束 非分离态用join
        total_c_h_lines += args[i].sum; // 累加结果
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

    return EXIT_SUCCESS;
}