#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
// 循序表
#define max 8
// 表是容器
typedef int ustype;
typedef struct
{
    ustype *data; // 容器
    int capacity; // 容量
    int length;   // 长度
} List;

void init(List *p)
{
    p->capacity = max;
    p->data = malloc(sizeof(ustype) * max);
    p->length = 0;
}

void grow(List *p)
{
    if (p->length == p->capacity)
    {
        // 扩容
        // 1.增长因子
        // 2.增加2倍 <<1
        // 3.增加一半 >>1 =1/2
        int grow = p->capacity >> 1;
        p->capacity += grow;
        p->data = realloc(p->data, sizeof(ustype) * p->capacity);
    }
    if (p->data == NULL)
    {
        perror("扩容失败");
        return;
    }
    printf("扩容到:%d\n", p->capacity);
}
void add(List *p, ustype e)
{
    if (p->length == p->capacity)
    {
        grow(p);
    }
    p->data[p->length++] = e;
}
int main()
{

    List list;
    // printf("%ld\n",sizeof(List));
    init(&list);
    add(&list, 100);
    add(&list, 200);
    add(&list, 300);
    add(&list, 100);
    add(&list, 200);
    add(&list, 300);
    add(&list, 100);
    add(&list, 200);
    grow(&list);
}