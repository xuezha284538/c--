// 复制操作
//
#include <stdio.h>
#include <ctype.h>
int main()
{
    FILE *in = fopen("io1.c", "r");
    FILE *out = fopen("io.txt", "w");
    if (in == NULL || out == NULL)
    {
        perror("无法打开文件");
        return 1;
    }
    // 读取
    int ch;
    while ((ch = fgetc(in)) != EOF)
    {
        // 加密操作 kaisa
        if (isalnum(ch))
        {
            ch++;
        }
        // 写
        fputc(ch, out);
    }
    printf("ok");
    fclose(in);
    fclose(out);
    return 0;
}