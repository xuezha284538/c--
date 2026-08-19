#include <stdio.h>
#include <ctype.h>
int main()
{
    FILE *fp = fopen("abc.txt", "r");
    if (fp == NULL)
    {
        printf("无法打开文件");
        return 1;
    }
    int by = 0, t = 0, word = 0;
    int ch = 0, f = 0;
    while ((ch = fgetc(fp)) != EOF)
    {
        by++;
        if (ch == '\n')
        {
            t++;
        }
        if (isalpha(ch))
        {
            if (!f)
            {
                word++;
                f = 1;
            }
        }
        else
        {
            f = 0;
        }
    }
    printf("%d %d %d", t, word, by);
    fclose(fp);
    return 0;
}