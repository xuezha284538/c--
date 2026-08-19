#include <stdio.h>
int r;
int *f(int *a, int *b)
{
    static int m;
    m = *a + *b;
    return &m;
}

void max(int *a, int size, int *r)
{
    *r = *a;
    // for (int i = 0; i < size; i++) //数组下标
    // {
    //     if (a[i] >= *r)
    //     {
    //         *r = a[i];
    //     }
    // }
    // for (int i = 0; i < size; i++) // 指针
    // {
    //     if (*(a + i) >= *r)
    //     {
    //         *r= *(a + i);
    //     }
    // }
    int *p = a;
    while (p < a + size)
    {
        if (*p >= *r)
        {
            *r = *p;
        }
        p++;
    }
}

void min(int *a, int size, int *r)
{
    *r = *a;
    // for (int i = 0; i < size; i++) //数组下标
    // {
    //     if (a[i] <= *r)
    //     {
    //         *r = a[i];
    //     }
    // }
    // for (int i = 0; i < size; i++) // 指针
    // {
    //     if (*(a + i) <= *r)
    //     {
    //         *r= *(a + i);
    //     }
    // }
    int *p = a;
    while (p < a + size)
    {
        if (*p <= *r)
        {
            *r = *p;
        }
        p++;
    }
}
int main()
{
    int m[] = {1, 2, 3, 4, 5, 99, 103, 56, 8, 7, 5, 1, 998, -1};
    int size = sizeof(m) / sizeof(m[0]);

    min(m, size, &r);
    
    printf("%d \n", r);
    return 0;
}