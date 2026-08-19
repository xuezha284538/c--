// 二路归并
#include <stdio.h>
#include <stdlib.h>
int min(int x, int y)
{
    return x < y ? x : y;
}
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
    int ch1[] = {1, 4, 7, 8, 9};
    int ch2[] = {2, 5, 6, 10, 11};

    int len1 = sizeof(ch1) / sizeof(ch1[0]);
    int len2 = sizeof(ch2) / sizeof(ch2[0]);

    int ch3[len1 + len2];
    int i = 0, j = 0, k = 0;
    while (i < len1 && j < len2)
    {
        if (ch1[i] <= ch2[j])
        {
            ch3[k] = ch1[i];
            i++;
            k++;
        }
        else if (ch1[i] >= ch2[j])
        {
            ch3[k] = ch2[j];
            j++;
            k++;
        }
    }
    while (i < len1)
    {
        ch3[k] = ch1[i];
        i++;
        k++;
    }
    while (j < len2)
    {
        ch3[k] = ch2[j];
        j++;
        k++;
    }
    show(ch3, len1 + len2);
    return 0;
}
