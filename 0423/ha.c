#include <stdio.h>
#include <stdlib.h>

#define max 19

int hax_f(int key, int mod)
{
    return key % mod;  // 除留余数法
}

void insert_hax(int hax[], int value, int mod)
{
    int index = hax_f(value, mod);
    while (hax[index] != 0) {
        index = (index + 1) % mod;  // 线性探测
    }
    hax[index] = value;
}

int search_hax(int hax[], int value, int mod)
{
    int index = hax_f(value, mod);
    while (hax[index] != 0) {
        if (hax[index] == value) {
            return index;
        }
        index = (index + 1) % mod;
    }
    return -1;
}

int main()
{
    int hax[max] = {0}, mod = 13;
    int values[] = {19, 14, 23, 1, 68, 20, 84, 27, 55, 11};
    int n = 10;

    for (int i = 0; i < n; i++) {
        insert_hax(hax, values[i], mod);
    }

    printf("{");
    for (int i = 0; i < mod; i++) {
        printf(" %d,", hax[i]);
    }
    printf("\b}");

    int key = 55;
    int pos = search_hax(hax, key, mod);
    if (pos != -1) {
        printf("\n查找 %d: 在索引 %d 位置找到\n", key, pos);
    } else {
        printf("\n查找 %d: 未找到\n", key);
    }

    return 0;
}
