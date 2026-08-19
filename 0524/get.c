#include <stdio.h>
#include "get.h"

/**
 * @brief 从标准输入读取字符串 获取字符串长度
 */
int get(char *p, int len)
{
    int c=len;
    while (1)
    {
        if (len > 0)
        {
            if (p[len - 1] == '\n')
            {
                p[len - 1] = '\0';
                break;
            }
        }
        p[len++] = getchar();
        c++;
    }
    return c - 1;
}
