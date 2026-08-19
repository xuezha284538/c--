#include "kmp.h"

/**
 * @brief 构建KMP算法的next数组
 */
void next1(int a[], char b[], int len)
{
    // a[]是next数组 b[]是模式串
    a[0] = -1;
    int i = 0, j = -1; // i后缀,j前缀终点 最大公共前缀
    while (i < len)
    {
        if (j == -1 || b[j] == b[i]) // 假设  a b a b c
        {                            //      0  1 2 3 4
            i++;                     //      -1 0 0 1 2 
            j++;
            a[i] = j;//保证最大
        }
        else//不相等就回退
        {
            j = a[j];//回退到最长公共前后缀"xx"对应的最大长度 依次缩小如"ab"是2 下次不相等就是"a" 是1
        }
    }
}

/**
 * @brief KMP字符串匹配算法
 */
int kmp(char *a, char *s, int *next, int s_len, int a_len)
{
    return kmp_from(a, s, next, s_len, a_len, 0);
}

/**
 * @brief 从指定位置开始的KMP字符串匹配算法
 */
int kmp_from(char *a, char *s, int *next, int s_len, int a_len, int start)
{
    int i = start, j = 0;

    while (i < a_len && j < s_len)
    {
        if (j == -1 || a[i] == s[j])
        {
            i++;
            j++;
        }
        else
        {
            j = next[j];
        }
    }
    if (j == s_len)
    {
        return i - j;
    }
    else
    {
        return -1;
    }
}