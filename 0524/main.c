#include <stdio.h>
#include <stdlib.h>

#define MAX 100000

#include "get.h"
#include "bf.h"
#include "kmp.h"
#include "helper.h"

/**
 * @brief 主函数：实现交互式字符串匹配工具
 */
int main(void)
{
    int a_len = 0, s_len = 0;
    char a[MAX], s[MAX - 1];//a是主字符串 s是模式字符串
    int mode; //匹配模式 1为单次 2为多次可重复 3为多次不可重复

    printf("=== kmp算法 ===\n\n");

    printf("请输入主字符串: ");
    a_len = get(a, a_len);
    printf("请输入模式字符串: ");
    s_len = get(s, s_len);

    int next[s_len+1];
    next1(next, s, s_len);

    printf("\n请选择匹配模式:\n");
    printf("1 - 单次匹配（只找第一个）\n");
    printf("2 - 多次重复匹配（允许重叠）\n");
    printf("3 - 多次不重复匹配（不允许重叠）\n");
    printf("请输入选项: ");
    scanf("%d", &mode);

    printf("\n匹配结果:\n");

    if (mode == 1)
    {
        int pos = kmp(a, s, next, s_len, a_len);
        if (pos != -1)
        {
            printf("找到匹配，位置: %d\n", pos);
        }
        else
        {
            printf("没有找到匹配\n");
        }
    }
    else if (mode == 2)
    {
        int pos = kmp(a, s, next, s_len, a_len);
        int count = 0;//记录匹配的次数
         while (pos != -1)
        {
            printf("位置 %d\n", pos);
            count++;
            pos = kmp_from(a, s, next, s_len, a_len, pos + 1);
        }
        if (count == 0)
        {
            printf("没有找到匹配\n");
        }
        else
        {
            printf("共找到 %d 个匹配\n", count);
        }
    }
    else if (mode == 3)
    {
        int pos = kmp(a, s, next, s_len, a_len);
        int count = 0;
        while (pos != -1)
        {
            printf("位置 %d\n", pos);
            count++;
            pos = kmp_from(a, s, next, s_len, a_len, pos + s_len);
        }
        if (count == 0)
        {
            printf("没有找到匹配\n");
        }
        else
        {
            printf("共找到 %d 个匹配\n", count);
        }
    }
    else
    {
        printf("无效\n");
    }

    printf("\n next数组: ");
    show1(next, s_len);
    printf("\n");

    return 0;
}