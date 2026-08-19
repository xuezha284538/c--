#include<iostream>
#include<string>
using namespace std;

class Cat
{
private:
    string name;
    int age;

public:
    Cat();
    Cat(string name);
    Cat(string name, int age);
    ~Cat();

    string getName();

    int getAge();
    void setAge(int n);

    void show();
};
int main()
{
    //访问修饰词

    return 0;
}