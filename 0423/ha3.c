#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 26
#define MAX_STR_LEN 100

char hash_table[TABLE_SIZE][MAX_STR_LEN];
int element_count = 0;

int hash(char* str, int mod)
{
    int hash_val = 0;
    while (*str) {
        hash_val = (hash_val * 31 + *str) % mod;
        str++;
    }
    return hash_val;
}

void init_hash_table()
{
    for (int i = 0; i < TABLE_SIZE; i++) {
        hash_table[i][0] = '\0';
    }
    element_count = 0;
}

void insert_hash_table(char* str)
{
    int index = hash(str, TABLE_SIZE);
    while (hash_table[index][0] != '\0') {
        index = (index + 1) % TABLE_SIZE;
    }
    strcpy(hash_table[index], str);
    element_count++;
}

int search_hash_table(char* str)
{
    int index = hash(str, TABLE_SIZE);
    int start = index;
    while (hash_table[index][0] != '\0') {
        if (strcmp(hash_table[index], str) == 0) {
            return index;
        }
        index = (index + 1) % TABLE_SIZE;
        if (index == start) {
            break;
        }
    }
    return -1;
}

void print_hash_table()
{
    printf("哈希表内容（大小: %d, 元素数: %d）:\n", TABLE_SIZE, element_count);
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (hash_table[i][0] != '\0') {
            printf("index %d: %s\n", i, hash_table[i]);
        } else {
            printf("index %d: NULL\n", i);
        }
    }
}

int main()
{
    init_hash_table();

    char* strings[] = {
        "hello", "world", "hash", "table", "dynamic",
        "linear", "probe", "load", "factor", "string",
        "test", "example", "resize"
    };
    int n = 13;

    for (int i = 0; i < n; i++) {
        insert_hash_table(strings[i]);
    }

    print_hash_table();

    char* search_str = "hash";
    int pos = search_hash_table(search_str);
    if (pos != -1) {
        printf("\n查找 '%s': 在索引 %d 位置找到\n", search_str, pos);
    } else {
        printf("\n查找 '%s': 未找到\n", search_str);
    }

    search_str = "notfound";
    pos = search_hash_table(search_str);
    if (pos != -1) {
        printf("查找 '%s': 在索引 %d 位置找到\n", search_str, pos);
    } else {
        printf("查找 '%s': 未找到\n", search_str);
    }

    return 0;
}
