#include <stdio.h>
#include <math.h>

#define max 15

void swap(int *p1, int *p2)
{
    int temp;
    temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

void dui(int *a, int mx, int cur)
{
    if (cur == -1)
        return;
    int large = cur;
    int left = 2 * cur + 1, right = 2 * cur + 2;
    if (left < mx && a[left] > a[large])
        large = left;
    if (right < mx && a[right] > a[large])
        large = right;
    if (large != cur)
    {
        swap(&a[cur], &a[large]);
        dui(a, mx, large);
    }
    else
    {
        dui(a, mx, cur - 1);
    }
}

void paixu(int a[], int mx)
{
    int size=mx,cur=(mx - 1) / 2;
    while(mx>0)
    {
    swap(&a[0],&a[mx-1]);
    mx--;
    dui(a,mx,cur);
    }

}

int main()
{
    int a[] = {5, 23, 8, 47, 51, 64, 77, 18, 107, 11, 102, 173, 514, 15};
    int mx = sizeof(a) / sizeof(a[0]);
    int cur, root = 0;
    cur = (mx - 1) / 2;
    dui(a, mx, cur);
     paixu(a,mx);
    for (int i = 0; i < mx; i++)
    {
        printf("%d,", a[i]);
    }
    printf("\n");
    return 0;
}
