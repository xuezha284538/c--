//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <openssl/evp.h>
#define max 512 * 1024
void evp_to_sha256(char *fliename)
{
    FILE *fp = fopen(fliename, "rb");
    FILE *out1 = fopen("meta.inf", "a");
    unsigned char hash[SHA256_DIGEST_LENGTH + 1];
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    // 参数 1.上下文
    // 2.算法
    // 3.引擎 null 默认引擎
    char *buf=malloc(max);
    EVP_DigestInit_ex(ctx, EVP_sha256(), NULL); // 通过更改evp_sha256 更改摘要模式算法 例如EVP_md5()
    fseek(fp,0,SEEK_END);
    long size=ftell(fp);
    fread(buf,1,size,fp);
    EVP_DigestUpdate(ctx, buf, size);
    int len;
    EVP_DigestFinal_ex(ctx, hash, &len);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        fprintf(out1, "%02x", hash[i]);
    }
    fprintf(out1, "\n");
    EVP_MD_CTX_free(ctx);
    fclose(fp);
    fclose(out1);
    free(buf);
}
void qie(char *filename)
{
    FILE *fp = fopen(filename, "r");

    if (!fp)
    {
        perror("无法打开文件");
        return;
    }
    fseek(fp, 0, SEEK_END); // 参数 第一个fp文件指针 第二个 跳过多少字节 第三个 光标位置  就是从光标位置跳过多少字节
    long size = ftell(fp);
    int n = (size % (max) == 0 ? size / (max) : size /(max) + 1);
     char *buf=malloc(max);
    rewind(fp);
    for (int i = 0; i < n; i++)
    {
        char *p = malloc(100);
        sprintf(p, "a%d.txt", i);
        FILE *out2 = fopen(p, "w");

        fread(buf, 1, max, fp);
        fwrite(buf, 1, size, out2);
        evp_to_sha256(p);

        //fseek(fp,(i+1)*(max), SEEK_SET);
        free(p);
        fclose(out2);
    }
    free(buf);
}



int main()
{
   
    qie("a.txt");
    evp_to_sha256("a.txt");
    return 0;
}
