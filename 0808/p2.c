#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/wait.h>
#define N 8
// unix std posix 可移植接口规范
int main()
{
    for(int i=0;i<N;i++)
    {
        int pid=fork();
        if(pid==0)
        {
            printf("worker:%d\t%d\n",getpid(),getppid());
            sleep(20);
            exit(0);
        }

    }
    for(int i =0;i<N;i++)
    {
        wait(NULL);
    }
    printf("master:end\n");

return 0;
}