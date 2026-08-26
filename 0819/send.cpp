#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdio>
#include <unistd.h>

int main()
{
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        perror("socket");
        return 1;
    }
    int ttl = 64;
    setsockopt(fd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl));
    int loop = 0; // 关闭回环：本机不再收到自己发的组播，避免重复
    setsockopt(fd, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));

    sockaddr_in dst{};
    dst.sin_family = AF_INET;
    dst.sin_port = htons(1234);
    inet_pton(AF_INET, "239.239.239.1", &dst.sin_addr);

    unsigned seq = 0;
    while (true)
    {
        char buf[64];
        int n = snprintf(buf, sizeof(buf), "hello multicast %u", seq++);
        sendto(fd, buf, n, 0, (sockaddr *)&dst, sizeof(dst));
        usleep(200000);
    }
}