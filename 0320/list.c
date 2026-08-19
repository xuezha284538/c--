#include <stdio.h>
#include<stdbool.h>
// 循序表
#define max 16
// 表是容器

typedef struct
{
    char data[max]; // 容量
    char *p;
    int length;     // 大小
} List;

void init(List *p)
{
    p->length = 0;
}

void add(List *p, char ch)
{
    // if (max == p->length)
    // {
    //     perror("满了\n");
    //     return ;
    // }

    p->data[p->length] = ch;
    p->length++;
}
//O(n)
void show(List *p)
{
    printf("{");
    for (int i = 0; i < p->length; i++)
    {
        printf("%c,", p->data[i]);
    }
    printf("\b}\n");
}
//O(n)
void insert(List *p, int lenth, char ch)
{
    // 移动
    for (int i = p->length - 1; i >= lenth; i--)
    {
        p->data[i + 1] = p->data[i];
    }
    // 插入
    p->data[lenth] = ch;
    p->length++;
}
// O(1)
char get(List *p, int index)
{
    // if (index>=p->length)
    // {
    //     /* code */return;
    // }
    return p->data[index];
}
// O(n)
int find(List *p, char ch)
{
    int index = -1;
    for (int i = 0; i <= p->length; i++)
    {
        if (p->data[i] == ch)
        {
            index = i;
        }
    }
    return index;
}
bool empty(List *p)
{
    return p->length==0;

}
void clean(List *p)
{
    p->length=0;
}
//O(n)
char remove1(List *p,int index)
{
    char ch=p->data[index];
    for (int i = index+1; i < p->length; i++)
    {
        p->data[i-1]=p->data[i];
    }
    p->length--;
    return ch;
}

int main()
{

    List list;
    // printf("%ld\n",sizeof(List));
    init(&list);
    add(&list, 'A');
    add(&list, 'B');
    add(&list, 'C');
    add(&list, 'D');
    add(&list, 'E');
    show(&list);
    insert(&list, 1, 'z');
    insert(&list, 1, 'x');
    insert(&list, 1, 'y');
    show(&list);
    char ch = get(&list, 4);
    printf("%c", ch);

    printf("%d", find(&list,'A'));
    show(&list);
    printf("%c",remove1(&list,1));
    show(&list);
    clean(&list);
    show(&list);

    return 0;
}
