# include<stdio.h>

// gcc hello.c 
 //生成 a.out 汇编输出
 //编译
 //gcc hello.c-o hello

 //汇编: gcc -S hello.c 生成hello.s
  //编译 gcc -c hello.s 生成hello.o
 //链接 gcc hello.o -o hello 生成 可执行文件 hello
 //执行:./hello


 int main()
{
    printf("hello c\n");
    printf("欢迎来到 fish \n");
    return 0;//返回os状态码 0表示正常结束
}
