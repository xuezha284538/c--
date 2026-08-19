#include <iostream>
#include <string>
#include"cat.h"
using namespace std;

int main()
{
    Cat c1;
    Cat c2("二狗");
    Cat c3("黑格尔",25);
    c2.setAge(19);
    // cout <<c1.getName()<<c1.getAge()<<endl;
    // cout <<c2.getName()<<endl;
    c1.show();
    c2.show();
    c3.show();
    return 0;
}