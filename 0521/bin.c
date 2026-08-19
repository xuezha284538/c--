// io1.c
// 字节操作
#include <stdio.h>

int main()
{
    // 文件名: 绝对路径:home/xuezha/a.txt 或者相对路径 a.txt ,../a.txt
    // 操作模式: r读 w写 a追加,r+,w+,rb 二进制,wb 二进制,
    FILE *fp = fopen("abc.txt", "r");
    if (fp == NULL)
    {
        printf("无法打开文件");
        return 1;
    }
    //
    printf("成功打开文件\n");
    // 读取 EOF  -1  文件结尾是EOF end of file
    int ch;
    while ((ch = fgetc(fp)) != EOF)
    {
        printf("%c", ch);
    }
    // 关闭
    fclose(fp);

    return 0;
}