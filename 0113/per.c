#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NAME_SIZE 16
typedef struct  // 定义一个新的数据类型 //结构体里 int 未初始化是零
{
    int year;
    int month;
    int day;
}date;

typedef struct Person
{
    char name[NAME_SIZE]; // 姓名 字段
    int age;              // 年龄
    date birthday; //
    struct Person *lover; // 可独立存在 即使person未定义
}Person;



int main(int argc, char const *argv[])
{
    struct Person p2={"non"};
    struct Person p1={"lo",21,{2004,03,03},&p2};
    //  p1.lover=&p2.name;
   

    printf("%s %d %d/%d/%d %s", p1.name, p1.age, p1.birthday.year, p1.birthday.month, p1.birthday.day, p1.lover->name);
    return 0;
}

