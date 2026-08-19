#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <openssl/sha.h>
#include <openssl/evp.h>

#define LISTEN_PORT 8888
#define BUF_SIZE    4096

// typedef struct {
//     int name_len;
//     long file_size;
// } FileHeader;

// 新版包头：与 send 端一致，携带摘要
typedef struct {
    int name_len;
    long file_size;
    char digest[65];   // SHA256 十六进制串，64 字符 + '\0'
} FileHeader;

// 计算文件 SHA256 摘要
void file_sha256_hex(const char *filename, char *out)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp) { perror("fopen"); return; }
    unsigned char hash[SHA256_DIGEST_LENGTH];
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, EVP_sha256(), NULL);
    char buf[BUF_SIZE];
    int n;
    while ((n = fread(buf, 1, sizeof(buf), fp)) > 0) {
        EVP_DigestUpdate(ctx, buf, n);
    }
    int len;
    EVP_DigestFinal_ex(ctx, hash, &len);
    EVP_MD_CTX_free(ctx);
    fclose(fp);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(out + i * 2, "%02x", hash[i]);
    }
    out[64] = '\0';
}

// 读取正好 n 字节
ssize_t readn(int fd, void *buf, size_t n)
{
    size_t left = n;
    char *p = buf;
    while (left > 0) {
        ssize_t r = read(fd, p, left);
        if (r < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        if (r == 0) return n - left;
        p += r; left -= r;
    }
    return n;
}

// 写满 n 字节
ssize_t writen(int fd, const void *buf, size_t n)
{
    size_t left = n;
    const char *p = buf;
    while (left > 0) {
        ssize_t w = write(fd, p, left);
        if (w < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        if (w == 0) break;
        p += w; left -= w;
    }
    return n - left;
}

int main(void)
{
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd < 0) {
        perror("socket");
        exit(1);
    }

    // 端口复用
    int opt = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(LISTEN_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(lfd, (struct sockaddr *)&addr, sizeof(addr));
    listen(lfd, 5);
    printf("服务端启动，监听端口 %d...\n", LISTEN_PORT);

    while (1) {
        struct sockaddr_in cli_addr;
        socklen_t cli_len = sizeof(cli_addr);
        int cfd = accept(lfd, (struct sockaddr *)&cli_addr, &cli_len);
        if (cfd < 0) {
            perror("accept");
            continue;
        }
        printf("客户端接入: %s:%d\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

        // 1. 接收头部
        FileHeader header;
        // read
        // recv(cfd, &header, sizeof(header), 0);
        readn(cfd, &header, sizeof(header));      // 改用 read

        // 2. 接收文件名
        char file_name[256] = {0};
        // recv(cfd, file_name, header.name_len, 0);
        readn(cfd, file_name, header.name_len);  // 改用 read
        printf("将要接收文件: %s，大小:%ld bytes，摘要:%s\n", file_name, header.file_size, header.digest);

        strcat(file_name, "_2");

        //  断点续传：检查已存在的断点文件，算出已接收字节数
        long resume_offset = 0;
        struct stat exist;
        if (stat(file_name, &exist) == 0) {
            resume_offset = exist.st_size;
            if (resume_offset > header.file_size) resume_offset = 0;  // 异常则重传
        }
        // 把断点偏移回传发送端
        writen(cfd, &resume_offset, sizeof(resume_offset));

        // 3. 创建文件用于写入：续传追加，全新截断
        int fd;
        if (resume_offset > 0)
            fd = open(file_name, O_WRONLY | O_APPEND);              // 续传：追加
        else
            fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644); // 全新：截断
        if (fd < 0) {
            perror("create file");
            close(cfd);
            continue;
        }

        // 4. 循环接收数据写入文件
        char buf[BUF_SIZE];
        long recv_total = resume_offset;     //  从断点继续累加
        ssize_t n;
        while (recv_total < header.file_size) {
            // n = recv(cfd, buf, BUF_SIZE, 0);
            n = read(cfd, buf, BUF_SIZE);    // 改用 read
            if (n <= 0) break;
            // write(fd, buf, n);
            writen(fd, buf, n);              //  改用 write
            recv_total += n;
            printf("\r已接收: %ld/%ld", recv_total, header.file_size);
        }
        printf("\n文件 %s 接收完成\n", file_name);

        // 接收端摘要校验
        int verify_ok = 0;       // 校验结果，用于回传发送端
        if (recv_total == header.file_size) {
            char recv_digest[65] = {0};
            file_sha256_hex(file_name, recv_digest);
            if (strcmp(recv_digest, header.digest) == 0) {
                printf(" 摘要校验通过，文件完整无误\n");
                verify_ok = 1;
            } else {
                printf(" 摘要校验失败！期望:%s\n   实际:%s\n", header.digest, recv_digest);
            }
        } else {
            printf("传输未完成（%ld/%ld），保留断点文件，可重连续传\n", recv_total, header.file_size);
        }

        close(fd);

        // shutdown 优化：把校验结果回传发送端，再半关闭写方向
        //   发送端已 shutdown(SHUT_WR) 表示“数据发完”，本端 read 拿到 EOF 即可退出循环；
        //   本端算完摘要，把结果写回发送端（利用对端仍保留的读方向），
        //   然后 shutdown(SHUT_WR) 通知发送端“回复完毕”，最后 close
        const char *result = verify_ok ? "OK\n" : "FAIL\n";
        writen(cfd, result, strlen(result));
        // shutdown
        shutdown(cfd, SHUT_WR);   // 半关闭写方向：数据已发完，告知对端 EOF
        close(cfd);
    }

    close(lfd);
    return 0;
}
