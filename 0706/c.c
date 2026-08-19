// client.c
//  socket-connetc-wirte/read-close
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h> //read write()
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    // 协议族 tcp/ip 套接字类型 :流 tcp
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    // 服务器的地址
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9001); // 主机字节序转大端  小端-> 大端无符号短整型
    // addr.sin_addr.s_addr=""
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    int c = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (c == -1)
    {
        printf("连接失败");
        return -1;
    }
    while (1)
    {

        // 读写 业务逻辑 协议规则
        // 模拟tlenet 功能
        char buf[1024];
        memset(buf, 0, sizeof(buf));
        printf("请输入: ");
        // 从标准输入 stdin
        fgets(buf, sizeof(buf), stdin);
        // write 发送写的长度? 字符串大小 整个buf
        size_t w_len = write(fd, buf, strlen(buf));
        if(w_len!=-1)
        {
            printf("发送: %s",buf);
        }
        while (w_len > 0 && (buf[w_len - 1] == '\n' || buf[w_len - 1] == '\r'))
        {
            buf[w_len - 1] = '\0';
            w_len--;
        }
        // read 接收 读取
        memset(buf, 0, sizeof(buf));
        size_t r_len = read(fd, buf, sizeof(buf));
        printf("接收到: %s\n", buf);
        if (strcmp(buf, "exit ok") == 0)
        {
            break;
        }
    }
    close(fd);
    return 0;
}