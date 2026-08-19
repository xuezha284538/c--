// base.cxx
// cxx

// input stream /output stream 输入输出流:字节序列
// cout
// cin
// 命名空间 std::
// g++ c.cxx -o c
#include <iostream>
// using namespace std;

int main()
{
    // printf("hello cxx\n");
    // std::cout << "hello c++\n";
    // int a;
    // double b;
    // //scanf
    // std::cin>>a>>b;
    // //endline
    // std::cout<<"a="<<a<<" ,b="<<b<<std::endl;
    // std::cout<<"a+b="<<a+b<<std::endl;

    // int val=0;
    // int sum=0;
    // std::cout<<"请输入数值:"<<std::endl;
    // while(std::cin>>val)
    // {
    //     sum+=val;
    // }
    // std::cout<<"sum:"<<sum<<std::endl;

    // int a=1;//c
    // int b={1};//列表初始化
    // int c(1);//构造函数初始化
    // int c{1};//列表初始化

    double n = 8.9;
    int m = 9;
    int v1 = n;
    int v2{m}; // 窄化类型
    auto v3 = n;
    std::cout << v1 << std::endl;
    std::cout << v2 << std::endl;
    std::cout << v3 << std::endl;

    return 0;
}
