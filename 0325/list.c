// 双链表
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>                
// 数据类型
typedef struct Node
{
    char data;         // 数据域
    struct Node *prev; // 指针域 前驱
    struct Node *next; // 指针域 后继
} node, Node;
// 别名 typedef struct Node dnode



// 链表
typedef struct dlist
{
    node *head; // 头指针
    node *tail; // 尾指针
    int len;    // 大小

} Dlist, list;

// 操作
void init(list *l);
node *add(list *l, char ch);
void show(list *l);
node *get(list *l, int ch);
node *insert(list *l, char ch, int index);
void delete(list *l, int index);
void reverse(list *l);
void clear(list *l);
node *find(list *l, char ch);

int main()
{
    list List;
    // list list1;
    init(&List);
    // init(&list1);
    add(&List, '1');
    add(&List, '2');
    add(&List, '3');
    add(&List, '4');
    insert(&List, 'h', 0);
    show(&List);
    delete(&List, 0);
    show(&List);
    
    // 测试翻转功能
    reverse(&List);
    show(&List);
    
    // 测试查找功能
    node *n = find(&List, '3');
    clear(&List);
    show(&List);
    
    return 0;
}
void init(list *l)
{
    // 创建头节点
    l->head = malloc(sizeof(node));
    l->head->prev = NULL; // 空表 首元节点
    l->head->next = NULL; // 空表 首元节点
    // 设置尾指针
    l->tail = l->head; // 尾指针指向头节点:空表
    l->len = 0;        // 大小为0
}
// O(1)
node *add(list *l, char ch)
{
    // 动态内存分配的堆空间
    node *new_node = malloc(sizeof(node));
    new_node->data = ch;
    // 尾插
    new_node->next = NULL;
    // 新节点指向最后一个元素
    new_node->prev = l->tail;
    // 尾指针(最后一个元素)的后继指向新节点
    l->tail->next = new_node;
    // 尾指针更新 指向现在的最后一个
    l->tail = new_node;
    // 大小加1
    l->len++;

    return new_node;
}
void show(list *l)
{

    node *p = l->head->next;
    printf("{ ");
    while (p != NULL)
    {
        printf("%c,", p->data);
        p = p->next;
    }
    // printf("\b");

    printf("}\b\n");
}

node *get(list *l, int ch)
{
    // TODO 优化从尾指针往前遍历
    // index=0;
    node *n = l->head;
    for (int i = 0; i < ch; i++)
    {
        n = n->next;
    }
    return n;
}

node *insert(list *l, char ch, int index)
{
    node *prev = get(l, index-1);
    // 当前位置的前一个结点
    node *new_node = malloc(sizeof(node)); // 新节点

    new_node->data = ch;

    new_node->next = prev->next;

    new_node->prev = prev; // 把前一个的结点的后继指向新节点的后继  使新节点的后继指向当前位置的后继

    prev->next->prev=new_node;//当前位置的后继的前驱 指向新节点

    prev->next = new_node;
    
    l->len++;
}
void delete(list *l, int index)
{
    node *n = get(l, index +1 ); // 当前

    n->prev->next = n->next; // 跳过目标结点
    n->next->prev = n->prev;

    free(n);
}
//O(N)
// 翻转双链表
void reverse(list *l)
{
    if(l->head->next==NULL || l->head->next->next==NULL|| l->len<=1)
    {
        return;
    }

    node *current = l->head->next; //第一个节点 首元节点
    node *next= NULL;// 当前节点的后继
    Node *prev=NULL;// 当前节点的前驱
    // 遍历链表，交换每个节点的前驱和后继指针
    while (current)
    {
        next = current->next;// 保存当前节点的后继
        // prev = current->prev;// 保存当前节点的前驱
        //{
        current->next= prev;// 当前节点的后继 指向当前节点的前驱
        prev = current;// 当前节点的前驱 指向当前节点的后继  
        //} 交换前驱后继
        current = next;// 更新当前节点为后继节点
    }
    // 交换头尾指针
    l->tail = l->head->next;
    l->head->next = prev;

    // 确保头节点的next指针指向新的首节点
    if (l->head->next != NULL)
    {
        l->head->next->prev = l->head;
    }
    
    // 确保尾节点的next指针为NULL
    if (l->tail != NULL)
    {
        l->tail->next = NULL;
    }
    
    printf("链表已翻转\n");
}

//O(N)
// 清空双链表
void clear(list *l)
{
    node *current = l->head->next;
    node *next=NULL;
    
    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
    
    // 重置链表状态
    l->head->next = NULL;
    l->tail = l->head;
    l->len = 0;
    
    printf("链表已清空\n");
}

//O(N)
// 查找元素
node *find(list *l, char ch)
{
    node *current = l->head->next;
    int position = 0;
    bool found = false;
    
    while (current != NULL)
    {
        if (current->data == ch)
        {
            printf("找到元素 '%c'，位置是 %d\n", ch, position);
            return current;
        }
        current = current->next;
        position++;
    }
    
    if (!found)
    {
        printf("没有找到元素 '%c'\n", ch);
    }
    return NULL;
}