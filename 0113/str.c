#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NAME_SIZE 16
typedef struct // 定义一个新的数据类型 //结构体里 int 未初始化是零
{
    char name[NAME_SIZE];//姓名 字段
    int age;             //年龄 
    Date birthday;       //生日
    Date *birth;
    
} Stu;

typedef struct // 定义一个新的数据类型 //结构体里 int 未初始化是零
{
    int year;
    int month;
    int day;
} Date;

void scan(Stu *ptr)
{
    if (!ptr)
    {
        printf("分配失败");
    }
    for (int i = 0; i < 4; i++)
    {
        // 构建字符串 学生0001  学生0002
        // char str[16];
        // sprintf(str, "学生 %04d", i + 1);
        // 赋值
        //  strcpy((ptr+i)->name,str);
        //  (ptr+i)->age=18+i;
        printf("请输入姓名与年龄  ");
        scanf("%s %d", (ptr)->name, &(ptr)->age);
        // printf("%s\t%d\n", (ptr)->name, (ptr)->age);
        ptr++;
    }
}

int main(int argc, char const *argv[])
{
    //-------------------------------------------------------------------
    // Stu *ptr = (Stu *)malloc(4 * sizeof(Stu));
    // scan(ptr);
    // // 打印
    // for (int i = 0; i < 4; i++)
    // {
    //     printf("%s\t%d\n", (ptr)->name, (ptr)->age);
    //     ptr++;
    // }

    // -----------------------------------------------------------------
    // Stu s1={"1",2}, a[4] = {{"xuezha", 21}, {"bob" }, {"syc" }},*p;
    // strncpy(a[3].name,"jack",NAME_SIZE-1);
    // a[3].age=12;

    // p=a;
    // strncpy((p+3)->name,"lpk",NAME_SIZE-1);
    // (p+3)->age=8;
    // for (int i =0 ; i <4 ; i++)
    // {
    //     printf("%s\t%d\n",p->name,p->age);p++;
    // }
    // //  strncpy(p->name,"ok",NAME_SIZE-1);
    // // (*p).age=19;

    // // printf("%s\t %d",(p)->name,(*p).age);
    return 0;
}
