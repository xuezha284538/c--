#include <stdio.h>
#include <stdlib.h>
int main()
{
    FILE *src = fopen("abc.txt", "rb");
    FILE *dest = fopen("b.txt", "wb");
    if (!src || !dest)
    {
        perror("无法打开文件");
        return 1;
    }
    char buf[7];
    int size;
    int c = 0;
    while ((size = fread(buf, 1, sizeof(buf), src)) > 0)
    {
        // dest=fopen("b.txt","wb");
        fwrite(buf, 1, size, dest); // 第三个参数写入的个数 不能是缓冲区大小
        c++;
        // fclose(dest);

        printf("%d次 读取%d 字节\n", c, size);
    }

    fclose(src);
    fclose(dest);

    return 0;
}