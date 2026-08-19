#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_SIZE 13
#define LOAD_FACTOR 0.7
#define MAX_STR_LEN 100

char** hash_table = NULL;
int table_size = INIT_SIZE;
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
    hash_table = (char**)calloc(table_size, sizeof(char*));
    for (int i = 0; i < table_size; i++) {
        hash_table[i] = NULL;
    }
    element_count = 0;
}

void resize_hash_table()
{
    int old_size = table_size;
    char** old_table = hash_table;

    table_size *= 2;
    hash_table = (char**)calloc(table_size, sizeof(char*));
    for (int i = 0; i < table_size; i++) {
        hash_table[i] = NULL;
    }
    element_count = 0;

    for (int i = 0; i < old_size; i++) {
        if (old_table[i] != NULL) {
            int index = hash(old_table[i], table_size);
            while (hash_table[index] != NULL) {
                index = (index + 1) % table_size;
            }
            hash_table[index] = strdup(old_table[i]);
            element_count++;
            free(old_table[i]);
        }
    }
    free(old_table);
}

void insert_hash_table(char* str)
{
    if ((float)element_count / table_size >= LOAD_FACTOR) {
        resize_hash_table();
    }

    int index = hash(str, table_size);
    while (hash_table[index] != NULL) {
        index = (index + 1) % table_size;
    }
    hash_table[index] = strdup(str);
    element_count++;
}

int search_hash_table(char* str)
{
    int index = hash(str, table_size);
    int start = index;
    while (hash_table[index] != NULL) {
        if (strcmp(hash_table[index], str) == 0) {
            return index;
        }
        index = (index + 1) % table_size;
        if (index == start) {
            break;
        }
    }
    return -1;
}

void print_hash_table()
{
    printf("哈希表内容（大小: %d, 元素数: %d）:\n", table_size, element_count);
    for (int i = 0; i < table_size; i++) {
        if (hash_table[i] != NULL) {
            printf("index %d: %s\n", i, hash_table[i]);
        } else {
            printf("index %d: NULL\n", i);
        }
    }
}

void free_hash_table()
{
    for (int i = 0; i < table_size; i++) {
        if (hash_table[i] != NULL) {
            free(hash_table[i]);
        }
    }
    free(hash_table);
    hash_table = NULL;
    table_size = INIT_SIZE;
    element_count = 0;
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

    free_hash_table();
    return 0;
}
