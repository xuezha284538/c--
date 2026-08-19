#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define max 100000
int get(char *p, int len)
{
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
    }
    return len - 1;
}
int find(char *a, char *s, int len1, int len2)
{
    int i, j, k;
    int a_len = len1;
    int s_len = len2;
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
int main()
{
    // int i,j,k;
    int a_len = 0, s_len = 0;
    char a[max], s[max - 1];
    a_len = get(a, a_len);
    s_len = get(s, s_len);
    printf("%d\n", find(a, s, a_len, s_len));
    // while(getchar()!='\n')
    // {
    //     a[a_len++]=getchar();
    // }
    // while(getchar()!='\n')
    // {
    //     s[s_len++]=getchar();
    // }

    return 0;
}