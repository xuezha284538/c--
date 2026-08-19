/**
 * 字符串复制
 *  for(int i;i<argc;i++)
    {
    printf("%d %s\n",i,argc[i]);
    }
    if(argc!=3)
    {
    printf("请加上俩个参数");
    }
    FILE *in= fopen("argc[1]", "r");
    FILE *out=fopen("argc2]", "w");
 * 命令: cc io5.c -o i
 *    :  ./i 文件名 文件名
 */

#include <stdio.h>

int main(int argc, char const *argv[])
{
    for (int i; i < argc; i++)
    {
        printf("%d %s\n", i, argv[i]);
    }
    if (argc != 3)
    {
        printf("请加上俩个参数");
    }
    FILE *in = fopen("argv[1]", "r");
    FILE *out = fopen("argv[2]", "w");
    if ((!in) || (!out))
    {
        perror("无法打开文件");
        return 1;
    }
    char buf[1024];
    //'/0'
    char *r;
    int c = 0;
    while (fgets(buf, sizeof(buf), in) != NULL)
    {
        // fputs(c+'1',out);
        fputs(buf, out);
        c++;
    }
    printf("%d", c);
    return 0;
}
