#include<iostream>
#include<string>
using namespace std;

int main()
{
    string s1;
    string s2="hello";
    string s3("hello");
    string s4(6,'a');
    // cin 给字符串赋值,读取到空格或者换行结束
    // cin >>s1;
    getline(cin,s1);//读取一行

    cout<<s1<<endl;
    cout<<s2<<endl;
    cout<<s3<<endl;
    cout<<s4<<endl;
    return 0;
}