#include <iostream>
#include <string>
#include <memory>
using namespace std;
struct Node
{
    int val;             //数据域
    //hared_ptr<Node> next; //指针域
    weak_ptr<Node>next;//弱指针 特殊的共享指针-----------
    //   struct node *next;
    Node(int v):val(v)
    {
        cout<<"Node()"<<endl;

    }
    ~Node()
    {
        cout<<"~node()"<<endl;
    }

};
//重载
void foo(){}
void foo(int v){}
void foo(int v ,int o){}

int main()
{
    //RAII
    auto n1=make_shared<Node>(5);
    auto n2=make_shared<Node>(6);//c++ c++11
    // shared_ptr<Node>n2(new Node(6)); 创建共享指针//c++ 14
    
    // cout<<n1.use_count()<<endl;
    // cout<<n2.use_count()<<endl;
    // //循环引用导致内存溢出
    // n1->next=n2; // n2计数器++ 弱指针不加加 解决循环引用
    // n2->next=n1; //
    // cout<<n1.use_count()<<endl;
    // cout<<n2.use_count()<<endl;

    weak_ptr<Node>n3=n1;
    cout<<n1->val << endl;
    cout<<n3.lock()->val << endl;//弱指针不能直接引用 弱指针要lock之后得到共享指针 就可以解引用

    return 0;
}