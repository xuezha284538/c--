#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define max 10
int a=1;
// 数据
typedef struct queue
{
    char data[max];
    int head,tail;

} Queue;

// typedef struct list
// {
//     // node *head;
//     // int len;
// } stack;

// 操作:init ,enqueue,denqueue,full,,empty,show
void init(Queue *q);
void enqueue(Queue *q,char ch);
char denqueue(Queue *q);
bool full(Queue *q);
int empty(Queue *q);
void show(Queue *q);
int size(Queue *q);
void clear(Queue *q);