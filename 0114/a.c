#include<stdio.h> //<>
#include"my.h"
// 宏名称 
#define PI  3.1415926535
#define C(R) 2*PI*(R) //有参建议给参数带括号
#define ADD(a,b) (a)+(b)
#define MUL(a,b) (a)-(b)
#define SWAP(a,b) a=a+b;b=a-b;a=a-b
//宏  参数无类型 解决函数重载的问题
#define MAX(a,b) a>b?a:b
#define MIN(a,b) a<b?a:b

#define PRINT(a) printf("%d\n",a)
#define PTS(a) printf("%s",a)
//`#`转字符串
#define TO_STR(a) #a
#define STR(a) TO_STR(a)
// ## 拼接字符串 链接
#define TO_CAT(a,b) a ## b 
#define CAT(a,b) TO_CAT(a,b)

double C1(int r)
{
    return 2*PI*r;
}

int main(int argc, char const *argv[])
{
    int a=100,b=200,c=300,d;
 
    printf("%d\n",MY_MAX(a,b));
    // -------------------------------------------------------------------------------------------------
    // int R =5; int a=1,b=2;
    //  SWAP(a,b);
    // // printf("%lf\n",2*PI*R+1);
    // // printf("%lf\n",C(R+1));
    // // printf("%lf",C1(R+1));
    // // printf("%d %d",a,b);
    // PRINT(MAX(a,b));
    // PRINT(b);
    // PTS("ASDFGHJ\n");
    // printf("%s=%d",STR(a+b),ADD(a,b));
    return 0;
}
