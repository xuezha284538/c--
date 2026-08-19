#include <stdio.h>
#include <math.h>

#define max 15

void swap(int* p1, int* p2)
{
    int temp;
    temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

void dui(int a[],int len)
{


}

int main()
{
    int a[max] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    int mx = sizeof(a) / sizeof(a[0]);
    int cur;

    int root = 0;
    cur = (mx - 1) / 2;
    int i = 0;
    int left = 2 * cur + 1, right = 2 * cur + 2;

    while (cur + 1) {
        for (int j = 0; j < log2(max + 1) + 1; j++) {
            left = 2 * cur + 1;
            right = 2 * cur + 2;
            if (left < mx && a[left] > a[cur]) {
                swap(&a[cur], &a[left]);
                cur = left;
            }
            if (right < mx && a[right] > a[cur]) {
                swap(&a[cur], &a[right]);
                cur = right;
            }
        }
        cur--;
    }

    for (; i < mx; i++) {
        printf("%d,", a[i]);
    }
    printf("\n");
    return 0;
}
