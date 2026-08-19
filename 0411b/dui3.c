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

void dui(int* a, int mx, int cur)
{
    if (cur == -1) return;
    int large = cur;
    int left = 2 * cur + 1, right = 2 * cur + 2;
    if (left < mx && a[left] > a[large])
        large = left;
    if (right < mx && a[right] > a[large])
        large = right;
    if (large != cur) {
        swap(&a[cur], &a[large]);
        dui(a, mx, large);
    } else {
        dui(a, mx, cur - 1);
    }
}

// void buildHeap(int *p, int n) {
//     // 从最后一个非叶子节点开始，向上调整
//     for (int i = n / 2 - 1; i >= 0; i--)
//         dui(p, n, i);
// }

int main()
{
    int a[] = { 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13, 14, 15 };
    int mx = sizeof(a) / sizeof(a[0]);
    int cur, root = 0;
    cur = (mx - 1) / 2;
    dui(a, mx, cur);
    // buildHeap(a,mx);
    for (int i = 0; i < mx; i++) {
        printf("%d,", a[i]);
    }
    printf("\n");
    return 0;
}
