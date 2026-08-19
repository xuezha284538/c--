#include<iostream>
#include<string>
using namespace std;
class Cat
{
private:
    std::string name;
    int age=1;
public:
    Cat();
    Cat(std::string name);
    Cat(string name,int age);
    ~Cat();
    string getName();
    int getAge();
    void setAge(int n);
    void show();

  
};