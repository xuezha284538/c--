#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdio>

int main()
{
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        perror("socket");
        return 1;
    }
    int yes = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(fd, (sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        return 1;
    }

    ip_mreq m{};
    inet_pton(AF_INET, "239.239.239.1", &m.imr_multiaddr);
    inet_pton(AF_INET, "192.168.76.131", &m.imr_interface); // ens33 的 IP，只在这块网卡加入组播
    if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &m, sizeof(m)) < 0)
    {
        perror("join");
        return 1;
    }
    printf("joined 239.239.239.1:1234 via 192.168.76.131, waiting...\n");

    char buf[128];
    while (true)
    {
        int n = recv(fd, buf, sizeof(buf), 0);
        if (n > 0)
            printf("recv %d bytes: %.*s\n", n, n, buf);
    }
}