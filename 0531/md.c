
// md.c
// 摘要算法
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <openssl/evp.h>
void evp_to_md5(char *str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    EVP_MD_CTX *ctx=EVP_MD_CTX_new();
    //参数 1.上下文
    //2.算法
    //3.引擎 null 默认引擎
    EVP_DigestInit_ex(ctx,EVP_sha256(),NULL);//通过更改evp_sha256 更改摘要模式算法 例如EVP_md5()
    //
    EVP_DigestUpdate(ctx,str,strlen(str));
    int len;
    EVP_DigestFinal_ex(ctx,hash,&len);
     EVP_MD_CTX_free(ctx);
    printf("MD5: ");
    show(hash,len);
}
void show(unsigned char *hash, int len)
{
    for (int i = 0; i < len; i++)
    {
        printf("%02x", hash[i]);
    }
    printf("\n");
}
// // 32
// // 已被破解，不可进行加密，推荐使用 sha 256
// // 可以用于文件验证、查重(快)
// void to_md5(char *str)
// {
//     // 16
//     unsigned char hash[MD5_DIGEST_LENGTH];
//     // 上下文 context - 环境
//     MD5_CTX ctx;
//     // 初始化上下文
//     MD5_Init(&ctx);
//     // 更新(多次，分片执行)
//     MD5_Update(&ctx, str, strlen(str));
//     // 生成哈希值(摘要信息)
//     MD5_Final(hash, &ctx);

//     // 转16进制->32
//     printf("MD5:\t");
//     show(hash, MD5_DIGEST_LENGTH);
// }

// // sha256

// void to_sha256(char *str)
// {
//     // 256/8
//     unsigned char hash[SHA256_DIGEST_LENGTH];
//     SHA256_CTX ctx;
//     SHA256_Init(&ctx);
//     SHA256_Update(&ctx, str, strlen(str));
//     SHA256_Final(hash, &ctx);
//     printf("SHA256:\t");
//     show(hash, SHA256_DIGEST_LENGTH);
// }

// void file_sha128(char *filename)
// {
//     FILE *fp = fopen(filename, "rb");
//     if (!fp)
//     {
//         perror("无法打开");
//         return;
//     }
//     // 20
//     unsigned char hash[SHA_DIGEST_LENGTH];
//     SHA_CTX ctx;
//     SHA1_Init(&ctx);
//     char buf[4096];
//     int n;
//     while ((n = fread(buf, 1, sizeof(buf), fp)) > 0)
//     {
//         SHA1_Update(&ctx, buf, n);
//     }
//     SHA1_Final(hash, &ctx);

//     // fseek(fp, 0, SEEK_END); // 参数 第一个fp文件指针 第二个 跳过多少字节 第三个 光标位置  就是从光标位置跳过多少字节
//     // long size = ftell(fp);
//     // rewind(fp);
//     // char *str = malloc(size);
//     // fread(str, 1, size, fp);

//     printf("file256: ");
//     show(hash, SHA_DIGEST_LENGTH);
//     fclose(fp);
// }

int main(int argc, char const *argv[])
{
    char *str = "a";
    evp_to_md5(str);


    // to_md5(str);
    // to_sha256(str);
    // file_sha128("0531.md");
    // file_sha128("a.txt");


    return 0;
}