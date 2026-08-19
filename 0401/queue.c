//circle queue
//循环(环形)队列
//1.空一格
//2.定义一个标识符 
//3.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define max 6

// 数据
typedef struct queue
{
    char data[max];
    int head,tail;
    // int tip;
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

int main()
{   
    Queue *q;
    init(q);
    enqueue(q,'a');
    enqueue(q,'0');
    enqueue(q,'1');
    enqueue(q,'2');
    enqueue(q,'3');
      show(q);
    denqueue(q);
    denqueue(q);
    denqueue(q);
    // denqueue(q);
    enqueue(q,'x');
    enqueue(q,'y');
    enqueue(q,'z');
    // clear(q);
       
        // enqueue(q,'a'); 
    // printf("%c\n",denqueue(q));
    // printf("%c\n",denqueue(q));

    show(q);

    return 0;
}

void init(Queue *q)
{
    q->head=0;
    q->tail=0;
    // q->tip=1;
}
void enqueue(Queue *q,char ch)
{   
    if(full(q))
    {
        printf("满了");
    }
    q->tail=(q->tail+1)%max;//q->tip=0;//满
    q->data[q->tail]=ch;
   
    
}

char denqueue(Queue *q)
{
    if(empty(q))
    {
        return ;
    }
   q->head=(q->head+1)%max; //q->tip=1;//没满
    return q->data[q->head];
}

bool full(Queue *q)
{
   if (empty(q))
   {
    return false;
   }
  
  return (q->tail+1)%max==q->head;//&&q->tip==0;
}
void show(Queue *q)
{
    if(empty(q))
    {
        printf("空的");
    }
    printf("{");
   
    
    for(int i=0;i<size(q);i++)
    {
        int index=(q->head+1+i)%max;
        printf("%c,",q->data[index]);
        
    }

    printf("\b}\n");
}
int empty(Queue *q)
{
    return (q->head==q->tail);//&&q->tip==1;
}
int size(Queue *q)
{
    return (q->tail-q->head+max)%max;
}
void clear(Queue *q)
{
    q->head=q->tail;
}