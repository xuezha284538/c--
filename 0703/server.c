#include <stdio.h>
#include <stdlib.h>
// memset
// memcpy
// memmove
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
// 地址
// 1.sys/un.h 本地 unix
// 2.netinet/in.h ipv4 网络 ipv6
// 3.arpa/inet.h ip 地址转换
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

void *handle(void *p)
{
   int c_fd= (int)p;
     while (1)
    {
        char buf[1024];
        memset(buf, 0, sizeof(buf));
        ssize_t n = read(c_fd, buf, sizeof(buf) - 1);
        if (n < 0)
        {
            perror("读取数据失败");
            break;
        }
        else if (n == 0)
        {
            printf("客户端断开连接\n");
            break;
        }

        // 去掉末尾的换行和回车符，方便判断 exit
        while (n > 0 && (buf[n - 1] == '\n' || buf[n - 1] == '\r'))
        {
            buf[n - 1] = '\0';
            n--;
        }

        // 空行跳过
        if (n == 0)
        {
            continue;
        }

        if (strcmp(buf, "exit") == 0)
        {
            printf("客户端请求退出,关闭连接\n");
            break;
        }

        printf("收到客户端请求: %s\n", buf);

        // 回显给客户端：把收到的内容原样发回去
        const char *prefix = "Server收到了:\t";
        char reply[2048];
        snprintf(reply, sizeof(reply), "%s%s\n", prefix, buf);
        write(c_fd, reply, strlen(reply));

    }
    return NULL;
    close(c_fd);
}
int main()
{
    // 创建服务端套接字
    // 1.地址族选择协议类型 AF_UNIX 本地 AF_INET ipv4 网络 AF_INET6 ipv6 网络
    // 2.套接字类型 SOCK_STREAM 流式 SOCK_DGRAM 数据报 udp SOCK_RAW 原始
    // 3.协议版本 0 表示默认版本
    // 4.返回值 套接字的文件描述符
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        perror("创建套接字失败");
        return -1;
    }

    // //端口复用：避免重启时 Address already in use
    // int opt = 1;
    // if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    // {
    //     perror("设置端口复用失败");
    //     close(fd);
    //     return -1;
    // }

    // struct sockaddr 通用地址结构体 用于绑定地址
    // struct sockaddr_in ipv4 地址结构体
    // struct sockaddr_un 本地 unix 地址结构
    struct sockaddr_in add;
    // 初始化 填充 0 清零
    memset(&add, 0, sizeof(add));
    //

    // 绑定地址
    // 1.地址族
    // 2.端口号 16位 2字节 无符号短整型
    // 3.ip地址 32位无符号整数
    add.sin_family = AF_INET; // ipv4 tcp/ip
    add.sin_port = htons(9210);
    add.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(fd, (struct sockaddr *)&add, sizeof(add)) == -1)
    {
        perror("绑定地址失败");
        close(fd);
        return -1;
    }

    // 监听
    // 1.套接字文件描述符
    // 2.队列长度
    if (listen(fd, 9) == -1)
    {
        perror("监听失败");
        close(fd);
        return -1;
    }
    printf("服务器准备就绪，监听端口 %d\n", ntohs(add.sin_port));
    //
    int flag = 1;
    while (flag)
    {
        printf("等待连接...\n");
        // 客户端地址 接收客户端地址
        struct sockaddr_in c_addr;
        memset(&c_addr, 0, sizeof(c_addr));
        socklen_t len = sizeof(c_addr);

        // 建立连接
        int c_fd = accept(fd, (struct sockaddr *)&c_addr, &len);
        if (c_fd == -1)
        {
            perror("建立连接失败");
            continue;
        }
        printf("客户端已连接: ip=%s, port=%d\n",
               inet_ntoa(c_addr.sin_addr), ntohs(c_addr.sin_port));
        // 创立线程 处理客户端请求 在新的线程中异步执行读写操作
        pthread_t tid;
        pthread_create(&tid, NULL, handle, (void *)c_fd);
        // pthread_join(tid,NULL);
        pthread_detach(tid);
        printf("当前连接已关闭\n\n");
    }
    // 关闭服务端套接字
    close(fd);
    return 0;
}