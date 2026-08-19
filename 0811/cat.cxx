#include "cat.h"
using namespace std;

Cat::Cat()
{
    cout << "构造函数\n";
    name = "咪咪";
    age = 1;
}
Cat::Cat(std::string s):name(s) //成员初始化
{
    cout << "构造函数\n";
    // name=name;
    //this.
    // this->name=name;
}
Cat::Cat(string name, int age)
{
    this->name=name;
    this->age=age;
}
Cat::~Cat()
{
    cout << "析构函数\n";
}

string Cat::getName()
{
    return name;
}

int Cat::getAge()
{
    return age;
}

void Cat::setAge(int n)
{
    if (n > 0 && n < 25)
    {
        age = n;
    }
}

void Cat::show()
{
    cout << "Cat: {名字：" << name << ", 年龄：" << age << "}" << endl;
}
