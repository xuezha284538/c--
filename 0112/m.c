/** mem.c
 * 动态内存管理
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct m
{
    char tel1[12];
    char *tel2;
};

char *cat(char *p, char *p1)
{

    int size = strlen(p) + strlen(p1) + 1;
    // char str[size]; 错误  局部变量    函数不能返回局部变量的地址
    char *str = (char *)malloc(size); // 动态内存分配
    if (str)
    {
        strcpy(str, p);
        strcat(str, p1);
    }


    return str;
}

int main(int argc, char const *argv[])
{
    struct m u1;
    u1.tel2 = "18687894561";
    strcpy(u1.tel1, "18453878451");
    //u1.tel2=malloc(12); //不填充0 快
    
    //menset(u1.tel2,0,12);//填充0 慢
    
    // printf("%s %s", u1.tel1, u1.tel2);
    //  char *p=malloc(12);
    //  char *p=calloc(1,12);
    
    //  for (int i = 0; i < 12; i++)
    //  {
    //     printf("%2d,%c\n",p[i],p[i]);
    //  }
     

    //---------------------------------------------------------
    //     char *p=cat("hello","c++");
    //     printf("%s %p",p,p);
    //     //
    //     free(p);

    //-----------------------------------------------------------------------
    // // 数组大小 确定
    // int a[6]; // 固定在6
    // // 动态内存分配
    // int *p = (int *)malloc(sizeof(int) * 6); // 24byte 空间
    // if (p == NULL)
    // {
    //     perror("内存不足 \n");
    //     return EXIT_FAILURE;
    // }

    // p[0] = 100;
    // p[1] = 200;
    // p[2] = 300;
    // p[3] = 400;
    // p[4] = 500;
    // p[5] = 600;
    // printf("before:%p \n", p);
    // realloc(p, sizeof(int) * 9);
    // p[6] = 700;
    // p[7] = 800;
    // p[8] = 900;
    // printf("after :%p \n", p);
    // // 释放
    // free(p);
    return EXIT_SUCCESS;
}
