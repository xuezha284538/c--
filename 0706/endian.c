// endian
#include <stdio.h>
int main()
{

    int n = 0x12345678;
    // p ->n的第一个字节
    char *p = (char *)&n;
    // %x 格式化十六进制 #是额外显示0x
    printf("%p %#x \n", p, *p);       // 0x78
    printf("%p %#x \n", p+1, *(p + 1)); // 0x56
    printf("%p %#x \n", p+2, *(p + 2)); // 0x34
    printf("%p %#x \n", p+3, *(p + 3)); // 0x12

    return 0;
}