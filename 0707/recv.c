// #include "my.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/un.h>
int main()
{
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    // 地址
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9527);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // 复用
    int opt = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    // 绑定
    bind(fd, (struct sockaddr *)&addr, sizeof(addr));

    // 接收
    // udp MTU(最大传输单元) 1500  包头
    // 固定的最大值 1500- 8-20
    char buf[1500 - 28];
    struct sockaddr_in c_addr;
    memset(&c_addr, 0, sizeof(c_addr));
    int len = sizeof(c_addr);
    //printf("接收中..");
    while (1)
    {

        if (strncmp(buf, "exit", 4) == 0)
        {
            //printf("终止接收.");
            memset(buf, 0, 1500 - 28);
            continue;
        }
    
        // 端口号 从网络字节序转换成主机字节序 n to h
        // 实际接受 r_len
        size_t r_len = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *)&c_addr, &len);
        // char *p = strchr(buf,'\n');
        // if (p != NULL)
        // {
        //     *p = '\0';
        // }

        int prot = ntohs(c_addr.sin_port);
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &c_addr.sin_addr, ip, sizeof(ip));

        printf("%s",buf);
        // printf("form : %s:%d  接收:%s", ip, prot, buf);

        memset(buf, 0, 1500 - 28);
        continue;
        //
    }

    close(fd);
    return 0;
}