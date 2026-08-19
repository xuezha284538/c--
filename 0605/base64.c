#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// OpenSSL 加密库核心头文件
#include <openssl/evp.h>
// OpenSSL BIO 流操作（用于 Base64 编码/解码）
#include <openssl/bio.h>
// OpenSSL 内存缓冲区操作
#include <openssl/buffer.h>

/**
 * @brief 对字符串进行 Base64 编码
 * @param str 待编码的原始字符串
 * @return 编码后的 Base64 字符串（需要手动 free 释放）
 */
char *encode(char *str)
{
    // 创建 Base64 编码过滤器 BIO（专门做 base64 编码的处理器）
    BIO *base64 = BIO_new(BIO_f_base64());
    // 创建内存 BIO（数据写入内存，而不是文件）
    BIO *bio = BIO_new(BIO_s_mem());

    // 串联 BIO：base64 过滤器 + 内存bio，形成处理流
    BIO_push(base64, bio);
    // 设置标志：Base64 编码结果**不自动换行**（标准单行格式）
    BIO_set_flags(base64, BIO_FLAGS_BASE64_NO_NL);

    // 把原始字符串写入流，自动完成 base64 编码
    BIO_write(base64, str, strlen(str));
    // 刷新缓冲区，确保所有数据都被编码输出
    BIO_flush(base64);

    // 定义内存缓冲区，用来接收编码后的数据
    BUF_MEM *buf;
    // 从 BIO 流中取出编码完成的数据
    BIO_get_mem_ptr(base64, &buf);

    // 动态分配内存：长度+1 存放字符串结束符 \0
    char *data = malloc(buf->length + 1);
    // 把编码结果拷贝到新内存
    memcpy(data, buf->data, buf->length);
    // 手动加结束符，变成合法 C 字符串
    data[buf->length] = '\0';

    // 释放整个 BIO 链（自动释放 base64 和 bio）
    BIO_free_all(base64);

    // 返回编码好的 base64 字符串
    return data;
}

/**
 * @brief Base64 字符串解码为原始数据
 * @param data 待解码的 Base64 字符串
 * @param len 输出参数：解码后的原始数据长度
 * @return 解码后的原始数据（需要手动 free 释放）
 */
char *decode(char *data, int *len)
{
    // 创建 Base64 **解码**过滤器 BIO
    BIO *b64 = BIO_new(BIO_f_base64());

    // 待解码的 base64 字符串长度
    int input_len = strlen(data);
    printf("输入Base64长度：%d\n", input_len);

    // 使用内存 BIO 包装待解码的字符串（数据源来自内存）
    BIO *bio = BIO_new_mem_buf(data, input_len);
    // 串联：base64 解码过滤器 + 数据源 bio
    bio = BIO_push(b64, bio);
    // 设置标志：解码时忽略换行（和编码对应）
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);

    // 分配内存：base64 解码后数据一定比原串短，用 input_len 足够
    char *buf = malloc(input_len);
    // 从流中读取解码后的数据，返回值=解码后的长度
    *len = BIO_read(b64, buf, input_len);

    printf("解码后数据长度：%d\n", *len);
    // 释放整个 BIO 链
    BIO_free_all(bio);

    // 返回解码后的原始数据
    return buf;
}

void encode_file(char *in_file, char *out_file)
{
    BIO *in = BIO_new_file(in_file, "rb");
    BIO *out = BIO_new_file(out_file, "w");
    // 过滤器
    char *base64 = BIO_new(BIO_f_base64());
    out = BIO_push(base64, out);
    char buf[1024 * 4];
    int n;
    while ((n = BIO_read(in, buf, sizeof(buf))) > 0)
    {
        BIO_write(out, buf, n);
    }
    BIO_flush(out);
    BIO_free_all(in);
    BIO_free_all(out);
    printf("ok");
}
void decode_file(char * in_file,char *out_file)
{
    BIO *in = BIO_new_file(in_file, "r");
    BIO *out = BIO_new_file(out_file, "wb");
    // 过滤器
    BIO *base64 = BIO_new(BIO_f_base64());
    in = BIO_push(base64, in);
    char buf[1024];
    int n;
    while((n=BIO_read(in,buf,sizeof(buf)))>0)
    {
        BIO_write(out,buf,n);
    
    }
    BIO_flush(out);
    BIO_free_all(in);
    BIO_free_all(out);
    printf("ok");
}
// 程序主入口
int main()
{
    // // 待编码的原始字符串

    // // 调用编码函数，得到 Base64 结果
    // char *str = "hello";

    // // 打印编码结果

    // // 释放 encode 函数中 malloc 分配的内存

    // // free(bas64);

    // // 待解码的 Base64 字符串（对应原文：hello）

    // char *data = encode(str);
    // printf("Base64 结果: %s\n", data);
    // // 用来接收解码后的数据长度
    // int len = 0;
    // // 调用解码函数
    // char *text = decode(data, &len);

    // // 打印解码结果
    // printf("解码结果：%s\n", text);

    // // 释放解码函数 malloc 的内存
    // free(text);

    // // 程序正常退出
    // 文件编码 与解码
    // data: image/jpg ;  base64,xxxxxx
    encode_file("tux.png", "tux.txt");
    // png,jpeg,jpg,gif,webp....
    //
    decode_file("tux.txt", "1.png");

    return 0;
}