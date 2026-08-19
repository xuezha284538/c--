#include <iostream>
namespace a
{
    int n{9}; // 列表初始化
    void hello()
    {
        std::cout << "hello a\n";
    }

}
namespace a
{
    int m = 91;
    namespace b
    {
        void bye()
        {
            std::cout << "bye" << std::endl;
        }

    }

}

using namespace a;
using namespace std;
// using std::endl,std::cout;
// using a::b::bye,a::hello,a::m;
// 定义一个类型 id c语言
typedef unsigned long int id;
// c++ 定义一个类型 类型的别名
using id = unsigned long int;

int main()
{

    //    std::cout<<"sum: "<<n+m<<std::endl;
    //     a::hello();
    //     a::b::bye();
    // 动态内存管理,堆空间
    // int *p=(int *)malloc(sizeof(int));
    // *p=42;

    // free(p);
    // p=NULL;

    int *p = new int; // 分配堆空间,未初始化
    cout << p << ", " << *p << endl;
    *p = 40;

    cout << p << ", " << *p << endl;
    delete p; // 释放堆空间
    cout << p << ", " << *p << endl;
    p = nullptr; // 清空指针 用nullptr 替代NULL

    int *p2 = new int(45); // 分配堆空间初始化为45
    *p2 = 99;              // 使用
    delete p2;             // 销毁
    p2 = nullptr;          // 赋值 赋空

    return 0;
}