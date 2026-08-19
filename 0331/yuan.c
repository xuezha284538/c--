// 元素去重
#include <stdio.h>
#include <stdlib.h>

void show(int ch[], int len)
{
    printf("{");
    for (int i = 0; i < len; i++)
    {
        printf("%d,", ch[i]);
    }
    printf("\b}\n");
}
int main()
{
    int c[] = {1, 2, 2, 3, 4, 2, 5, 6, 3, 4, 2, 1, 6};
    int len = sizeof(c) / sizeof(c[0]);
    int haxi[100] = {0};

    int arr[len], i = 0, k = 0;
    while (i < len)
    {
        if (!haxi[c[i]])
        {
            arr[k] = c[i];
            k++;
            haxi[c[i]] = 1;
        }
        i++;
    }

    show(arr, k);

    return 0;
}