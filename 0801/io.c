/**
 * io.c
 * 系统调用
 * posix std 技术规范 可移植的操作系统的接口标准技术规范
 */
#include<unistd.h>
#include<stdio.h>
//file control operations
#include<fcntl.h>
#include<sys/stat.h>
int main()
{
//     // 0 stdin
//     // 1 stdout
//     // 2 stderr
//     // 3 新打开的
//    // char buf[16]="hello posix";
//     // if(write(3,buf,16)!=16)
//     // {
//     //     write(2,"error",6);
//     // }
//     char buf[16]={0};
//     //memset
//     int n=read(0,buf,sizeof(buf)-1);
//     write(1,buf,n);


int fd=open("xyz.txt",O_WRONLY | O_CREAT,0777);
//int fd=open("xyz.txt",O_WRONLY | O_CREAT,S_IRWXU | S_IRWXG |S_IRWXO);
    printf("%d\n",fd);
    char buf[]="hello posix";
    int n=write(fd,buf,sizeof(buf));
     printf("%d\n",n);
    close(fd);
    return 0;
}