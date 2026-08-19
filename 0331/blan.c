
// 荷兰国旗问题

#include <stdio.h>
#include <stdlib.h>

void swap(int *p, int *p1)
{
    int temp = *p1;
    *p1 = *p;
    *p = temp;
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
    int ch[] = {1, 2, 0, 0, 1, 2, 2, 0, 1, 2, 0, 1};
    int len = sizeof(ch) / sizeof(ch[0]);
    int r = 0;
    int w = 0;
    int b = len - 1;
    while (w <= b)
    {
        if (ch[w] == 0)
        {
            swap(&ch[w], &ch[r]);
            r++;
            w++;
        }

        else if (ch[w] == 1)
        {
            w++;
        }
        else
        {
            swap(&ch[w], &ch[b]);
            b--;
        }
    }

    show(ch, len);

    return 0;
}