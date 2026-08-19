#include"my.h"
typedef struct
{
    int local_port;
    char *peer_ip;
    int peer_port;
} thread_arg_t;
void * recvr(void *p)
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
        // printf("接收中..");
        while (1)
        {

            if (strncmp(buf, "exit", 4) == 0)
            {
                // printf("终止接收.");
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

            printf("接收:%s", buf);
            // printf("form : %s:%d  接收:%s", ip, prot, buf);

            memset(buf, 0, 1500 - 28);
            continue;
            //
        }

        close(fd);
        return NULL;
    
}
void * sendr(void *p)
{
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    // max MTU 1500-
    char msg[1500 - 28];
    // 接收方地址   端口以及ip
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9527);                     // 接收方端口
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr); // 接收方ip

    while (1)
    {

        memset(msg, 0, sizeof(msg));
        printf("开始发送\n");
        fgets(msg, sizeof(msg), stdin);
        size_t len = sendto(fd, msg, strlen(msg), 0, (struct sockaddr *)&addr, sizeof(addr));
        // printf("发送了:%s", msg);
        if (strncmp(msg, "exit", 4) == 0)
        {
            printf("退出发送. \n");
            break;
        }
    }

    // 关闭
    close(fd);

    return NULL;
}
int main(int argc, char *argv[])
{
    while(1)
    {
    if (argc != 4)
    {
        printf("用法: %s 本地端口 对方IP 对方端口\n", argv[0]);
        return -1;
    }

    // 把参数打包传给线程
    thread_arg_t arg;
    arg.local_port = atoi(argv[1]);
    arg.peer_ip = argv[2];
    arg.peer_port = atoi(argv[3]);

    pthread_t tid_recv, tid_send;

    // 启动收线程
    pthread_create(&tid_recv, NULL, recvr, &arg);
    // 启动发线程
    pthread_create(&tid_send, NULL, sendr, &arg);

    // 等send线程退出（你输入exit时结束）
  pthread_join(tid_send, NULL);
    }
    return 0;
}