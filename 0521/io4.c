/**
 * 读取字符串
 *
 *
 *
 */

#include <stdio.h>

int main()
{
    FILE *fp = fopen("io4.c", "r");
    if (!fp)
    {
        perror("无法打开文件");
        return 1;
    }
    char buf[1024];
    //'/0'
    char *r;
    int c = 0;
    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        printf("%s", buf);
        c++;
    }
    printf("%d", c);
    return 0;
}
