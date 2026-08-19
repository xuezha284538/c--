#include <stdio.h>
#include <stdlib.h>
int main(int argc, char const *argv[])
{
    FILE *fp = fopen("abc.txt", "r");
    FILE *out1 = fopen("abc_1_4", "w");
    FILE *out2 = fopen("abc_2_4", "w");
    FILE *out3 = fopen("abc_3_4", "w");
    FILE *out4 = fopen("abc_4_4", "w");

    if (!fp || !out1 || !out2 || !out3 || !out4)
    {
        perror("无法打开文件");
        return 1;
    }
    // 光标从文件末尾跳过0个字节
    fseek(fp, 0, SEEK_END); // 参数 第一个fp文件指针 第二个 跳过多少字节 第三个 光标位置  就是从光标位置跳过多少字节
    long size = ftell(fp);
    int s_4 = (size % 4 == 0 ? size / 4 : size / 4 + 1);
    int len = s_4;
    // 回到文件开头
    rewind(fp);

    char *buf = malloc(s_4);
    int n;
    int c = 0;
    while ((n = fread(buf, 1, s_4, fp)) > 0)
    {
        c++;
        switch (c)
        {
        case 1:
            fwrite(buf, 1, n, out1); // 第三个参数写入的个数 不能是缓冲区大小
            printf("%d次 读取%d 字节\n", c, s_4);
            break;
        case 2:
            fwrite(buf, 1, n, out2);
            printf("%d次 读取%d 字节\n", c, s_4);
            break;
        case 3:
            fwrite(buf, 1, n, out3);
            printf("%d次 读取%d 字节\n", c, s_4);
            break;
        case 4:
            fwrite(buf, 1, n, out4);
            printf("%d次 读取%d 字节\n", c, s_4);
            break;
        }
        if (c == 4)
        {
            break;
        }
        size -= s_4;
        s_4 = (size % (4 - c) == 0 ? size / (4 - c) : size / (4 - c) + 1);

        
    }
    fclose(fp);
    fclose(out1);
    fclose(out2);
    fclose(out3);
    fclose(out4);

    return 0;
}