// server .c
// socket -bind-lisen-accept-read-write-close
#include"my.h"
int port; 
char ip[INET_ADDRSTRLEN];
struct sockaddr_in c_addr;
void * handle(void *p)
{
  char  msg[64];
  char  buf[1024];
    int c_fd = (int)p;

    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    getpeername(c_fd, (struct sockaddr *)&addr, &len);

    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr.sin_addr, ip, INET_ADDRSTRLEN);
    int port = ntohs(addr.sin_port);
    printf("ip:%s 端口%d 已连接\n", ip, port);
    while (1)
    {
        strcpy(msg, "已收到");
        memset(buf, 0, 1024);
        int n = read(c_fd, buf, sizeof(buf));
        if (n != -1)
        {
            printf("接收到 :%s", buf);
        }

        if (n > 0 && (strncmp(buf, "exit", 4) == 0))
        {
            strcpy(msg, "exit ok");
            write(c_fd, msg, strlen(msg));
            printf("ip:%s 端口%d 断开连接\n", ip, port);
            break;
        }
        // memset(buf, 0, 1024);
        write(c_fd, msg, strlen(msg));
    }

    /* msg is stack-allocated now */
    close(c_fd);
}
int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    // 地址复用
    int opt = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    // bind
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9001);
    bind(fd, (struct sockaddr *)&addr, sizeof(addr));
    // 监听
    listen(fd, 9);
    printf("服务器启动 \n");


  
    while (1)
    {
        memset(&c_addr, 0, sizeof(c_addr));
        int len = sizeof(c_addr);
        int c_fd = accept(fd, (struct sockaddr *)&c_addr, &len);
        // 创立线程 处理客户端请求 在新的线程中异步执行读写操作
        pthread_t tid;
        pthread_create(&tid, NULL, handle, (void *)c_fd);
        pthread_detach(tid);
        // pthread_join(tid,NULL);

        //     if (c_fd == -1)
        //     {
        //         printf("连接失败\n");
        //         break;
        //     }
        //    else
        //     {

        // }
        // 读写

        // while (n > 0 && (buf[n - 1] == '\n' || buf[n - 1] == '\r'))
        // {
        //     buf[n - 1] = '\0';
        //     n--;
        // }
     
        
    }
    close(fd);
    return 0;
}