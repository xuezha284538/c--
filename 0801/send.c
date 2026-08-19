#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/stat.h>
// #include <sys/sendfile.h>   // 原本用 sendfile 零拷贝，现改用 read/write，不再需要
#include <netinet/in.h>
#include <arpa/inet.h>
// OpenSSL 摘要头文件
#include <openssl/sha.h>
#include <openssl/evp.h>

#define SERV_IP    "127.0.0.1"
#define SERV_PORT  8888
#define BUF_SIZE   4096

// 传输包头：先发送文件名长度、文件大小
// typedef struct {
//     int name_len;
//     long file_size;
//     // char md[32];
// } FileHeader;

// 新版包头：增加摘要字段，发送时生成、接收时校验
typedef struct {
    int name_len;
    long file_size;
    char digest[65];   // SHA256 十六进制串，64 字符 + '\0'
} FileHeader;

// 计算文件 SHA256 摘要，输出 64 位十六进制字符串
// 参考自 0531/w.c 的 evp_to_sha256()
void file_sha256_hex(const char *filename, char *out)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp) { perror("fopen"); return; }
    unsigned char hash[SHA256_DIGEST_LENGTH];          // 32 字节
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, EVP_sha256(), NULL);        // 选 sha256 算法
    char buf[BUF_SIZE];
    int n;
    // 分块更新，避免一次性读入大文件
    while ((n = fread(buf, 1, sizeof(buf), fp)) > 0) {
        EVP_DigestUpdate(ctx, buf, n);
    }
    int len;
    EVP_DigestFinal_ex(ctx, hash, &len);
    EVP_MD_CTX_free(ctx);
    fclose(fp);
    // 二进制 -> 两位十六进制字符串
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(out + i * 2, "%02x", hash[i]);
    }
    out[64] = '\0';
}

// 读取正好 n 字节（解决 read 短读：read 可能一次返回少于请求量）
ssize_t readn(int fd, void *buf, size_t n)
{
    size_t left = n;
    char *p = buf;
    while (left > 0) {
        ssize_t r = read(fd, p, left);
        if (r < 0) {
            if (errno == EINTR) continue;   // 被信号打断，重试
            return -1;
        }
        if (r == 0) return n - left;        // 对端关闭
        p += r; left -= r;
    }
    return n;
}

// 确保 write 写满 n 字节（write 也可能只写一部分）
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

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s upload_file_path\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    const char *file_path = argv[1];

    // 1. 创建socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, SERV_IP, &serv_addr.sin_addr);

    // 连接服务器
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        close(sockfd);
        exit(1);
    }

    // 2. 打开待上传文件
    int file_fd = open(file_path, O_RDONLY);
    if (file_fd < 0) {
        perror("open file");
        close(sockfd);
        exit(1);
    }

    // 获取文件大小
    struct stat st;
    fstat(file_fd, &st);
    long total_size = st.st_size;
    char file_name[256] = {0};
    // 截取文件名（去掉路径）
    char *p = strrchr(file_path, '/');
    if (p) strcpy(file_name, p + 1);
    else strcpy(file_name, file_path);
    int name_len = strlen(file_name);

    // 发送时生成摘要（整份文件的 SHA256）
    char digest[65] = {0};
    file_sha256_hex(file_path, digest);

    // 3. 发送文件头部信息
    FileHeader header = {
        .name_len = name_len,
        .file_size = total_size
    };
    strcpy(header.digest, digest);          // 把摘要放进包头
    // write SHA256
    // send socket
    // send(sockfd, &header, sizeof(header), 0);
    // send(sockfd, file_name, name_len, 0);
    writen(sockfd, &header, sizeof(header));    //  改用 write
    writen(sockfd, file_name, name_len);       //  改用 write

    printf("准备上传文件: %s, size: %ld bytes, sha256: %s\n", file_name, total_size, digest);

    //  断点续传：读取服务端告知的“已接收字节数”，从断点继续发
    long resume_offset = 0;
    readn(sockfd, &resume_offset, sizeof(resume_offset));
    if (resume_offset > total_size) resume_offset = 0;   // 异常保护
    if (resume_offset > 0)
        printf("服务端已有 %ld 字节，从断点续传\n", resume_offset);
    else
        printf("全新上传，从头开始\n");

    // 4. sendfile 零拷贝发送文件 —— 改为 read/write 循环
    // 偏移量参数传入/传出
    // off_t offset = 0;
    // ssize_t send_len;
    // long sent = 0;
    // while (sent < total_size) {
    //     // 一次最多发送BUF_SIZE
    //     send_len = sendfile(sockfd, file_fd, &offset, BUF_SIZE);
    //     if (send_len <= 0) {
    //         perror("sendfile");
    //         break;
    //     }
    //     sent += send_len;
    //     printf("\r已发送: %ld/%ld", sent, total_size);
    // }
    lseek(file_fd, resume_offset, SEEK_SET);   //  跳到断点位置
    long sent = resume_offset;                  // 已发量从断点起算
    char buf[BUF_SIZE];
    ssize_t n;
    while (sent < total_size) {
        n = read(file_fd, buf, BUF_SIZE);       // 从文件 read
        if (n <= 0) { perror("read file"); break; }
        if (writen(sockfd, buf, n) < 0) { perror("write socket"); break; } // ★ write 到 socket
        sent += n;
        printf("\r已发送: %ld/%ld", sent, total_size);
    }
    printf("\n文件发送完毕\n");

    //  shutdown 优化：数据发完后半关闭写方向 SHUT_WR
    //   作用1：通知接收端“没有更多数据了”，接收端 read 返回 0(EOF) 干净退出循环
    //         比单纯靠“字节计数相等”判断更可靠，避免对端死等
    //   作用2：仍保留读方向，可以接收接收端回传的摘要校验结果
    //   对比 close：close 是双向全关闭，发不出/收不到任何东西；
    //              shutdown(SHUT_WR) 只关写方向，读方向照常可用
    shutdown(sockfd, SHUT_WR);

    // 读取服务端的摘要校验结果（接收端算完摘要会回传 "OK" / "FAIL"）
    char reply[64] = {0};
    ssize_t r, got = 0;
    while ((r = read(sockfd, reply + got, sizeof(reply) - 1 - got)) > 0) {
        got += r;
    }
    if (got > 0)
        printf("服务端校验结果: %s\n", reply);
    else
        printf("未收到服务端校验结果\n");

    close(file_fd);
    close(sockfd);
    return 0;
}
