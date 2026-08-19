#include <stdio.h>
int max(int a, int b)
{
    return a > b ? a : b;
}

int main(int argc, char const *argv[])
{
    int a = 1;
    int b = 2;
    // 常量指针  值不可修改
    int const *p = &a; // 指向a的常量指针 可以访问a的值 不可修改a的值 可以访问不同地址
                       //*p = 3;  //错误
    p = &b;            // 正确  可以指向不同地址

    // 指针常量  地址不可修改
    int *const q = &a; // 指向a的指针常量 可以访问a的值
    *q = 3;            // 正确  可以修改值
    // q = &b; //错误  不可以指向不同地址
    int r = max(a, b);
    return 0;
}
