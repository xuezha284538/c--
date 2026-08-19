#include <stdio.h>
#include <string.h>

int main()
{
    int num;
    // 1. 定义存储16进制字符的数组 (小写)，这是基准字符集
    char p[] = "0123456789ABCDEF";
    // 2. 定义字符数组，存储转换后的16进制余数(逆序)，int最大转16进制最多8位
    char s[20] = {0};
    // 定义下标，记录res_buf的存储位置
    int index = 0;

    // 输入一个整数
    printf("请输入一个正整数：");
    scanf("%d", &num);
    // 备份原值，用于打印提示
    int stc_num = num;

    // 3. 核心：十进制转十六进制 【除16取余法】
    if(num == 0)
    {
        // 特殊情况：输入0，直接存0
        s[index++] = *p; // 指针偏移0位，等价p
    }
    else
    {
        while(num > 0)
        {
            int yu = num % 16;  // 对16取余，得到0~15的余数
            num = num / 16;            // 商继续循环运算，直到商为0
            
            // ============ 指针偏移获取对应字符  ============
            // p是字符数组首地址(指针)，+yu 余数 就是指针向后偏移 yu 位
            // * 解引用这个偏移后的指针，得到对应16进制字符
            s[index++] = *(p + yu); 
        }
    }

    // 4. 打印结果：取余得到的字符是【逆序存储】的，需要倒序输出
    printf("整数 %d 对应的十六进制是: 0x", stc_num);
    for(int i = index - 1; i >= 0; i--)
    {
        printf("%c", s[i]);
    }
    printf("\n");

    return 0;
}