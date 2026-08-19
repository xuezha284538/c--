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

void show(int a[], int len)
{
    printf("{");
    for (int i = 0; i < len; i++)
    {
        printf("%d,", a[i]);
    }
    printf("\b}\n");
}

int kmp(char *a, char *s, int *next, int s_len, int a_len)
{
    int i = 0, j = 0, k = 0;

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
        return -1;
}

int main()
{
    int i, j, k, n;
    int a_len = 0, s_len = 0;
    char a[max], s[max - 1];

    a_len = get(a, a_len);
    s_len = get(s, s_len);
    int next[s_len];
    // printf("%d\n", bf(a, s, a_len, s_len));
   next[0]=0;
    for (i = 1; i < s_len; i++)
    {
        int temp = 0;
        for (j = 0; j < i; j++)
        {
            int m = 1,n=j;
            for (k = 0; k <= j; k++)
            {
                if (s[k] != s[i-n])
                {
            
                    break;
                }
                if (s[k] == s[i-n]&&n==1)
                {
            
                    temp = j;
                }
                n--;
            }
          
                
        }
        next[i-1] = temp;printf("%d",temp);
    }
    show(next, s_len);
    printf("%d\n", kmp(a, s, next, s_len, a_len));

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
