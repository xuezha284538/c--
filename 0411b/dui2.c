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

int main()
{
    int a[] = { 10, 8, 7, 9, 6, 5, 4, 1, 2, 3 };
    int mx = sizeof(a) / sizeof(a[0]);
    int cur, root = 0;
    // cur=root;
    cur = (mx - 1) / 2;
    int left = 2 * cur + 1, right = 2 * cur + 2;
    while (cur + 1) {
        int large = cur;
        left = 2 * cur + 1;
        right = 2 * cur + 2;
        if (left < mx && a[left] > a[large]) {
            large = left;  // swap(&a[cur],&a[large]);
        }
        if (right < mx && a[large] < a[right]) {
            large = right;  // swap(&a[cur],&a[large]);
        }

        if (large != cur) {
            swap(&a[cur], &a[large]);
            cur = large;
        } else {
            cur--;
        }
    }

    for (int i = 0; i < mx; i++) {
        printf("%d,", a[i]);
    }
    printf("\n");
    return 0;
}
