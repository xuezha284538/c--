#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char const *argv[])
{
    int a, b, c, j, o; 
    int p[9];
    for (a = 123; a < 329; a++)
    {
        //   p=calloc(4,9);
        b = 2 * a;
        c = 3 * a;
        int c1[10] = {0};//记录出现次数的数组 每次清零 防止干扰
        int temp = a; //临时变量 存储a的值
        for (j = 0; temp != 0; temp /= 10) //取出百位 十位 和个位
        {
            p[j] = temp % 10;
            j++;
        }
        temp = b;
        for (j = 3; temp != 0; temp /= 10)
        {
            p[j] = temp % 10;
            j++;
        }
        temp = c;
        for (j = 6; temp != 0; temp /= 10)
        {
            p[j] = temp % 10;
            j++;
        }

        for (int i = 0; i < 9; i++) //循环遍历 
        {
            int d = p[i]; // 因为i是1-9  所以大小为c1[10] 
            c1[d]++; //出现一次加一
        }
        o = 0;
        for (int i = 0; i < 10; i++)
        {
            if (c1[i] > 1||c1[0]==1)//所以只要有一个数组里面的值超过一就代表重复 
            {
                o = 1; //重复标志
            }
        }

        if (!o)
        {
            printf("a=%d,b=%d,cd=%d \n", a, b, c);
        }
    }

    return 0;
}