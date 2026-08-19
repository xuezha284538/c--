// 单链表 实现栈 头插法
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 数据
typedef struct node
{
    char data;
    struct node *next;

} node;

typedef struct list
{
    node *head;
    int len;
} stack;

// 操作
void init(stack *s);
void push(stack *s, char ch);
char pop(stack *s);
char top(stack *s);
void clean(stack *s);
bool empty(stack *s);
void show(stack *s);

int main()
{
    stack s;
    init(&s);
    push(&s, 'a');
    push(&s, '3');
    push(&s, 'a');
    push(&s, 'a');
    show(&s);
    pop(&s);
    show(&s);
    printf("%c\n", top(&s));
    
    if (empty(&s))
    {
        printf("空栈\n");
    }
    else
    {
        printf("非空栈\n");
        show(&s);
    }
    clean(&s);
    show(&s);
    return 0;
}

void init(stack *s)
{
    s->head = malloc(sizeof(node));
    s->head->next = NULL;
    s->len = 0;
    // printf("1");
}

void push(stack *s, char ch)
{
    node *new = malloc(sizeof(node));
    if (new == NULL)
    {
        // 处理内存分配失败
        return;
    }
    new->data = ch;
    new->next = s->head->next;
    s->head->next = new;
    s->len++;
    // printf("1");
}

char pop(stack *s)
{
     if (s->head->next == NULL) 
    {
        return '\0';  // 用字符表示错误
    }
    char ch = s->head->next->data;
    node *curren = s->head->next;
    s->head->next = s->head->next->next;
    free(curren);
    s->len--;
    return ch;
}

char top(stack *s)
{
    if (s->head->next == NULL) 
    {
        // 处理空栈情况
        return '\0';  
    }
    return s->head->next->data;
}

void clean(stack *s)
{
    node *new = s->head->next;
    while (new)
    {
        node *temp = new->next;
        free(new);
        new = temp;
    }
    s->head->next = NULL;
    s->len = 0;
}

bool empty(stack *s)
{
    return s->len == 0;
}

void show(stack *s)
{
    node *new = s->head->next;

    if (s->head->next == NULL)
    {
        printf("{}");
        return;
    }
    else
    {
        printf("{");
        while (new)
        {
            printf("%c,", new->data);
            new = new->next;
        }
        printf("\b}\n");
    }
}
