#include <stdio.h>

/// @brief pointer.c`
//值 :基本类型 复制一份
void swap(int *n, int *m)
{
    int temp = *n;
    *n = *m;
    *m = temp;
}

int main(int argc, char const *argv[])
{

    //-------------------------------------------------------
    // char c = 'a';
    // int a = 1;
    // double g = 8.0;

    // char *p2 = &c;
    // int *p1 = &a;
    // double *p3 = &g;

    // printf("大小(c)=%zu\n", sizeof(c));
    // printf("大小(a)=%zu\n", sizeof(a));
    // printf("大小(g)=%zu\n", sizeof(g));
    // printf("指针大小(p2)=%zu,指针的值%p,指向的空间数据%c\n", sizeof(p2), p2, *p2);
    // printf("指针大小(p1)=%zu,指针的值%p,指向的空间数据%d\n", sizeof(p1), p1, *p1);
    // printf("指针大小(p3)=%zu,指针的值%p,指向的空间数据%lf\n", sizeof(p3), p3, *p3);
    return 0;
}
