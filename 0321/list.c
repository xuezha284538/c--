// 尾插法 实现单链表
#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    char data;         // 数据域
    struct Node *next; // 指针域 后继
    // struct Node*prev;//指针域 前驱
} node, Node;

// 定义链表 :头节点(哨兵节点) 尾指针
typedef struct list
{
    Node *head; // 头节点,创建
    Node *tail; // 尾指针,空表指向头节点,非空指向终端节点
} list;
// list..h
void init(list *l);
node *add(list *l, char ch);
void show(list *l);
void clear(list *p);
node *get(list *l, int ch);
node *insert(list *l, char ch, int index);
void delete(list *l, int index);
void find(list *l, char ch);
void resever(list *l);
int main()
{
    list List;
    list list1;
    init(&List);
    init(&list1);
    add(&List, 'q');
    add(&List, '2');
    add(&List, 'e');
    add(&List, 'o');
    show(&List);
    printf("%c\n", get(&List, 2)->data);
    insert(&List, '4', 3);
    show(&List);

    delete(&List, 0);
    show(&List);

    resever(&List);
    show(&List);
    printf("%c",List.tail->data);
    // clear(&List);

    // show(&List);
    // clear(&list1);

    return 0;
}
// list.c
// O(1)
void init(list *l)
{
    // 创建头节点
    l->head = malloc(sizeof(node));
    l->head->next = NULL; // 空表 首元节点
    // 设置尾指针
    l->tail = l->head; // 尾指针指向头节点:空表
}
// O(1)
node *add(list *l, char ch)
{
    // 动态内存分配的堆空间
    node *new_node = malloc(sizeof(node));

    new_node->data = ch;
    new_node->next = NULL;

    l->tail->next = new_node;
    l->tail = new_node;

    return new_node;
}
// O(N)
void show(list *l)
{
    node *p = l->head->next;
    printf("{ ");
    while (p != NULL)
    {
        printf("%c,", p->data);
        p = p->next;
    }
    printf("\b");

    printf("}\n");
}
// O(N)
void clear(list *p)
{
    node *current = p->head->next;
    while (current != NULL)
    {
        node *next = current->next;
        free(current);
        current = next;
    }
    p->head->next = NULL;
    p->tail = p->head;
}
// O(N)
node *get(list *l, int ch)
{
    node *n = l->head;
    for (int i = 0; i < ch; i++)
    {
        n = n->next;
    }
    return n;
}
// O(1)
node *insert(list *l, char ch, int index)
{
    node *prev = get(l, index + -1);       // 当前位置的前一个结点
    node *new_node = malloc(sizeof(node)); // 新节点
    new_node->data = ch;
    new_node->next = prev->next; // 把前一个的结点的后继指向新节点的后继  使新节点的后继指向当前位置的后继
    prev->next = new_node;
}

void delete(list *l, int index)
{
    node *prev = get(l, index - 1); // 获得前驱
    node *n = prev->next;           // 获得目标结点
    prev->next = prev->next->next;  // 跳过目标结点
    free(n);
}
// O(N)
void find(list *l, char ch)
{
    node *new = l->head->next;
    int p = 0;

    while (new != NULL)
    {

        if (new->data == ch)
        {
            printf("找到了%c在第%d", new->data, p);
        }
        new = new->next;
        p++;
    }
}
// O(N)
void resever(list *l)
{
    node *prev = NULL;
    node *current = l->head->next;
    node *next = NULL;
    while (current != NULL)
    {
        next = current->next; // 保存下一个节点
        current->next = prev; // 反转指针方向
        prev = current;       // 前一个节点后移
        current = next;       // 当前节点后移
    }
   
    l->tail=l->head->next;
    l->head->next=prev;

    // int po = 0;
    // int i = 0;
    // node *p = l->head->next;
    // while (p != NULL)
    // {
    //     p = p->next;
    //     po++;
    // }
    // char arr[po];
    // p = l->head;
    // while (p != NULL)
    // {
    //     arr[i] = p->data;
    //     p = p->next;
    //     i++;
    // }
    // clear(l);
    // for (po; po > 0; po--)
    // {
    //     char ch = arr[po];
    //     add(l, ch);
    // }
}