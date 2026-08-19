#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/wait.h>
// unix std posix 可移植接口规范
int main()
{
    int pid = fork();
    if (pid == -1)
    {
        perror("创建失败");
        return EXIT_FAILURE;
       
    }
    if (pid == 0)
    {
        // 子进程空间
        printf("worker:%d\t%d\n",getpid(),getppid());
        // sleep(30);
        //exec 
        //execl 多个参数逗号分隔
        //execv 数组
        //execlp("wc","wc","-l","p2.c",NULL);

        //1.路径
        //2.程序名
        //3.参数
        //4.NULL 标识参数结束
        // execlp("uname", "uname", "-r", NULL);
        execlp("./m","m",NULL);
        exit(42);
    }
    else
    {
        //主进程
        
        int status;
        waitpid(pid,&status,WUNTRACED);
        if(WIFEXITED(status))
        {
            int val=WEXITSTATUS(status);
            printf("val=%d\n",val);
        }
        printf("master:%d\t%d\n", getpid(), getppid());
        printf("pid:%d\n",pid);
        // sleep(90);
    }

    return 0;
}