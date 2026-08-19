// send .c
// 发送方
#include "my.h"

    int main()
    {
        int fd = socket(AF_INET, SOCK_DGRAM, 0);
        // max MTU 1500-
        char msg[1500-28];
        // 接收方地址   端口以及ip
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(9527);                     // 接收方端口
        inet_pton(AF_INET, "122.51.242.187", &addr.sin_addr); // 接收方ip
        
        while(1)
        {
            memset(msg, 0, sizeof(msg));
            printf(":");
            fgets(msg, sizeof(msg), stdin);
            size_t len = sendto(fd, msg, strlen(msg), 0, (struct sockaddr *)&addr, sizeof(addr));
        // printf("发送了:%s", msg);
            if(strncmp(msg,"exit",4)==0)
            {
                printf("退出.");
                break;

            }
        }

        // 关闭
        close(fd);

        return 0;
    }