




#include<stdio.h>
#include<stdlib.h>
typedef struct Node
{
    int data;
    struct Node *next;
} Node, *LinkList;
void init(LinkList l)
{
     l= malloc(sizeof(Node));
     l->next=NULL;
     l->data=0;
}
void insert(LinkList l, int data)
{
    Node *new = malloc(sizeof(Node));
    new->data = data;
    new->next=l->next;
    l->next=new;
    // printf("%d,",new->data);
}
void del_x(LinkList L,int x)
{
    Node *prev = L;
    Node *current = L->next;
    if(current==NULL) return ;
    while (current != NULL)
    {
        if (current->data == x)
        { 
            prev->next = current->next;
            free(current);
            return del_x(L,x);
        }
        prev = current;
        current = current->next;
    }
  
}

void show(LinkList l)
{
    Node *p=l->next;
    while (p)
    {
        printf("%d,", p->data);
        p = p->next;
    }
    printf("\n");
}
int main()
{
    LinkList list1;
    init(list1);
    insert(list1, 12);
    insert(list1, 322);
    insert(list1, 2);
    insert(list1, 2);
    insert(list1, 2);
    insert(list1, 2);
    insert(list1, 322);

  show(list1);
 
  del_x( list1, 2);
   show(list1);
    return 0;
}


/*

void del_x(LinkList L,int x)
{
    Node *prev = L;
    Node *current = L->next;
    if(current==NULL) return ;
    while (current != NULL)
    {
        if (current->data == x)
        { 
            prev->next = current->next;
            free(current);
            return del_x(L,x);
        }
        prev = current;
        current = current->next;
    }
  
}*/