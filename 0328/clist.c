// 循环单链表
// 循环双链表

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>                
// 数据类型
typedef struct Node
{
    char data;         // 数据域
    // struct Node *prev; // 指针域 前驱
    struct Node *next; // 指针域 后继
    
} node,Node;

// 别名 typedef struct Node dnode

//链表
typedef struct clist
{
    struct Node *head;         // 头指针
    // struct Node *prev; // 指针域 前驱
    struct Node *tail; // 尾指针    
    int len;    // 大小
} list,Clist;



//操作
void init(list *l);
void add(list *l, char ch);
void show(list *l);
void clear(list *l);
Node *get(list *l,int index);
void insert(list *l,int index,char ch);

void reverse(list *l);
int main()
{
    list *List;
    init(List);
    add( List, '1');
    add( List, '2');
    add( List, '3');
    add( List, '4');
    
    show( List);
    printf("%c",get(List,1)->data);
    insert(List,5,'h');
    show( List);
    
    clear(List);
    return 0;
}

void init(list *l)
{
    l->head=malloc(sizeof(Node));
    if(l->head==NULL)
    {
        return ;
    }
    l->tail = l->head;
    l->head->next = l->head;
    l->len = 0;
}
void add(list *l, char ch)
{
    Node *new=malloc(sizeof(Node));
    new->data=ch;
    new->next=l->head;//新节点指向头节点

    //尾插法
    l->tail->next=new;
    l->tail=new;

    l->len++;
}
void show(list *l)
{
    Node *current=l->head->next;
    if(l->len<1)
    {
        printf("{ }\n");
        return;
    }

    printf("{ ");
    while(current!=l->head)
    {
        printf("%c,",current->data);
        current=current->next;
    }
    printf("}\b\n");
}
void clear(list *l)
{ 
 Node *current = l->head->next;
  while(current==l->head)
  { 
    Node *temp=current->next;
    free(current);
    current=temp;
  }
  free(l->head);
  init(l);

}
Node *get(list *l,int index)
{
    Node *new=l->head;
    for(int i=0;i<l->len;i++)
    {
        if(i==index)
        {
            return new;
        }
        new=new->next;

    }
    return l->head;

}
void insert(list *l,int index,char ch)
{
    Node *new=malloc(sizeof(Node));
    new->data=ch;
    
    if(l->len==index-1)
    {
       add(l,ch);
    }
    else
    {
    Node *prev=get(l,index-1);
    new->next=prev->next;
    prev->next=new;+
    l->len++;
    }
    
}
void reverse(list *l)
{
   
}
