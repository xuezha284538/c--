#include "helper.h"
#include <stdio.h>

/**
 * @brief 格式化打印整型数组
 */
void show(int a[], int len)
{
    printf("{");
    for (int i = 0; i < len; i++)
    {
        printf("%d,", a[i]);
    }
    printf("\b}\n");
}

/**
 * @brief 打印next数组（比实际长度多1个元素）
 */
void show1(int a[], int len)
{
    for (int i = 0; i < len + 1; i++)
    {
        printf("%d,", a[i]);
    }
}

// /**
//  * @brief 初始化整型数组为0
//  */
// // void init(int s[], int len)
// // {
// //     for (int i = 0; i < len; i++)
// //     {
// //         s[i] = 0;
// //     }
// // }
