#include <stdio.h>
int main()
{
    //-------闰年
    // 1)被4整除且不被100整除
    // 2)被400整除
    int year;
    printf("请输入一个年份: ");
    scanf("%d", &year);
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
    {
        printf("%d 是闰年\n", year);
    }
    else
    {
        printf("%d 不是闰年\n", year);
    }

    //-----------------************--------------------**************----------------
    // int score;char grade;
    // printf("请输入你的成绩: ");
    // scanf("%d", &score);
    // if (score >= 0 && score <= 100)
    // {
    //     if (score >= 90)
    //     {
    //         grade='A';
    //     }
    //     else if (score >=80)
    //     {
    //         grade='B';
    //     }
    //     else if (score >=70)
    //     {
    //         grade='C';
    //     }
    //     else if (score >=60)
    //     {
    //         grade='D';
    //     }
    //     else
    //     {
    //         grade='E';
    //     }

    //         printf("你的成绩是:%d 级别:%c\n", score, grade);
    // }

    // else
    // {
    //     printf("输入的成绩无效\n");
    // }

    
    //-----------------************--------------------**************----------------
    // int age;
    // printf("请输入你的年龄: ");
    // scanf("%d", &age);
    // if (age >= 18 && age < 70)
    // {
    //     printf("有效年龄\n");
    // }
    // else
    // {
    //     printf("无效年龄\n");
    // }

    return 0;
}
