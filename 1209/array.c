#include <stdio.h>
int main(int argc, char const *argv[])
{ //    声明数组变量
    char grade[5] = {'E', 'D', 'C', 'B', 'A'};
    int n[5] = {60, 70, 80, 90, 100};
    int m = 0;
    printf("请输入成绩:");
    scanf("%d", &m);
    for (int i = 0; i < 5; i++)
    {
        if (m > n[i])
            continue;

        else
        {
            printf("成绩等级为:%c\n", grade[i]);
            break;
        }
    }

    return 0;
}
