#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    //strtok token
    char s1[]="alice,NULL,xuezha,bob -jack";
    char s2[sizeof(s1)];
    strcpy(s2,s1);
    char *tok= strtok(s2,", ;-");
    while (tok!=NULL)
    {
        printf("%s\t",tok);
        tok =strtok(NULL,", ;-");
    }
    printf("over:%s",s1);
    

    // strchr strrchr  正序查找 反向查找   -------------------------------------
    // char s1[] = {"java"};
    // char s2[] = {"89c+a123"};
    // char *p = strrchr(s2, '1');
    // if (p == NULL)
    // {
    //     printf("没有找到");
    // }
    // else
    // {
    //     printf("%s\t", p);
    //     printf("%ld", p - s2);
    // }
    // strcmp ----------------------------------------------------------------
    //  char s1[] = {"abc6"};

    // char s2[] = {"abd"};

    // int r = strcmp(s1, s2);
    // printf("%d", r);

    // strcpy , strcat  -------------------------------------------------------
    // char a[20] = {"hello "};
    // char b[] = {"xuezha"};
    // int size = strlen(a) + strlen(b) + 1;
    // char c[size];

    // strcat(c, a); //追加 拼接
    // strcat(c, b);
    // printf("%s\t", c);

    // strncpy(a, " bye", sizeof(a) - 1); // 拷贝 覆盖
    // strcat(b, a);
    // printf("%s", b);

    return 0;
}