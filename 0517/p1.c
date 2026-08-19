#include <stdio.h>
#include <stdlib.h>
typedef struct Node
{
    int data;
    struct Node *next;
} Node;
typedef struct List
{
    struct Node *head;
    struct Node *tail;
} LinkList;

void init(LinkList *l)
{
    l->head = malloc(sizeof(Node));
    l->head->next = NULL;
    l->tail = l->head;
}
void insert(LinkList *l, int data)
{
    Node *new = malloc(sizeof(Node));
    new->data = data;
    new->next = NULL;
    l->tail->next = new;
    l->tail = new;
    // printf("%d,",new->data);
}
//单链表逆序（翻转）在这-----------
Node *reverse(LinkList *l)
{
    Node *prev = NULL;
    Node *cur = l->head->next;
    Node *next = NULL;
    while (cur != NULL)
    {
        next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }

    l->tail = l->head->next;
    l->head->next = prev;
    return l->head;
}
//-------------------------------
void show(LinkList *l)
{
    Node *cur = l->head->next;
    while (cur)
    {
        printf("%d,", cur->data);
        cur = cur->next;
    }
    printf("\n");
}
int main()
{
    LinkList list1;
    init(&list1);
    insert(&list1, 12);
    insert(&list1, 322);
    insert(&list1, 2);

    show(&list1);
    reverse(&list1);
    show(&list1);
    return 0;
}
