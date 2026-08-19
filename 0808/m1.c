#include<stdio.h>
int main(int argc,char const *argv[])
{
    if(argc<2)
    {
        printf("参数不足");
        return 1;
    }
    printf("这是另外的程序:%s\n",argv[1]);
    return 0;
}