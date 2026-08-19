// 字符写入
#include <stdio.h>
int main()
{
    // w 写 覆盖的形式  a 追加的形式
    FILE *fp = fopen("b.txt", "a");

    if (fp == NULL)
    {
        perror("无法打开文件 \n"); // 没有权限
    }
    // 写入
    fputc('S', fp);
    // 关闭
    fclose(fp);

    return 0;
}