#include <iostream>

void swap(int *p, int *p2)
{
    int t = *p;
    *p = *p2;
    *p2 = t;
}

void swap2(int &p, int &p2)
{
    int t = p;
    p = p2;
    p2 = t;
}
int main()
{
    int a = 42;
    int b = 23;
    // int *p;//野指针
    // p=&a;//指向a
    // *p=43;
    // p=&b;..

    //&引用 &标识r 是引用 变量的别名
    // 引用 声明时要赋值 不可以再次赋值
    // int &r=a;

    // std::cout<<a<<","<<r<<std::endl;
    // r = b;
    // int &r2=r;
    // std::cout<<a<<","<<r<<std::endl;
    // swap(&a,&b);
    swap2(a, b);
    std::cout << "a=" << a << ",b=" << b << std::endl;
    return 0;
}