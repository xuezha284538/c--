//cp.c
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<sys/stat.h>
#include<stdlib.h>

int main (int argc ,char const* argv[])
{
    if(argc!=3)
    {
        perror("参数不足");
        return 1;
    }
    int fd_in,fd_out;
    //源
    fd_in=open(argv[1],O_RDONLY);
    //目标
    fd_out=open(argv[2],O_WRONLY |O_CREAT,0660);
    char buf[1024*4]={0};
    char ch;
    int n=0;
    struct stat s;
    // 获取文件描述符的文件信息 大小 时间 用户 权限
    fstat(fd_in,&s);
    printf("%ld %ld %ld \n",s.st_size,s.st_blksize,s.st_ctime);
    while( (n=read(fd_in,buf,sizeof(buf)))>0)
    {
        write(fd_out,buf,n);
  
    }
    printf("ok\n");
    close(fd_in);
    close(fd_out);
  

    return 0 ;
}