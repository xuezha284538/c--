#include "bf.h"

/**
 * @brief 暴力匹配算法（Brute Force）
 */
int bf(char *a, char *s, int a_len, int s_len)
{
    int i, j, k;

    for (i = 0; i <= a_len - s_len; i++)
    {
        k = i;
        for (j = 0; j < s_len; j++)
        {
            if (a[k] == s[j])
            {
                k++;
            }
            else
            {
                break;
            }
        }
        if (j == s_len)
        {
            return i;
        }
    }
    return -1;
}
