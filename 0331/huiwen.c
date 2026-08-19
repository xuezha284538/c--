// 判断单链表回文
//  尾插法 实现单链表
#include <stdio.h>
#include <stdlib.h>
// 节点

typedef struct Node
{
    char data;         // 数据域
    struct Node *next; // 指针域
} Node, node;

typedef struct // 最小单链表 头节点加尾指针
{
    Node *head;
    Node *r;
    int length;
} linkedlist;

void init(linkedlist *p);
void add(linkedlist *p, char ch);
void show(linkedlist *p);
void find(linkedlist *p, char ch);
void clear(linkedlist *p);
void delete(linkedlist *p, char ch);
node *revers(node *p);
node *mid(node *p);
void is(linkedlist *p);
linkedlist droad(linkedlist *p,linkedlist *p1);
int main()
{
    linkedlist list;
    init(&list);
    add(&list, 'a');
    add(&list, 'b');
    add(&list, 'c');
    add(&list, 'b');
    add(&list, 'a');
    add(&list, 'a');
    add(&list, 'a');
    add(&list, 'a');
    show(&list);

    is(&list);
    // revers(mid(list.head));
    // show(&list);

    // find(&list, '8');

    // delete(&list, '5');
    // printf("删除后链表: ");
    // show(&list);

    return 0;
}

void init(linkedlist *p)
{
    // 创建头节点
    p->head = malloc(sizeof(Node));
    if (p->head == NULL)
    {
        printf("内存分配失败\n");
        exit(1);
    }
    // 空表
    p->head->next = NULL;
    // 尾指针指向头结点
    p->r = p->head;
    // 初始化长度
    p->length = 0;
}

void add(linkedlist *p, char ch)
{
    // 创建新节点
    node *new_node = malloc(sizeof(node)); // 修复：创建新节点
    if (new_node == NULL)
    {
        printf("内存分配失败\n");
        exit(1);
    }
    // 存入数据域
    new_node->data = ch;
    // 新节点赋空
    new_node->next = NULL;
    // 最后一个元素的后继指向新节点
    p->r->next = new_node;
    // 尾指针指向新节点
    p->r = new_node;
    p->length++;
}
void show(linkedlist *p)
{
    node *node = p->head->next;
    printf("{");
    if (node != NULL)
    {
        while (node != NULL)
        {
            printf("%c,", node->data);
            node = node->next;
        }
        printf("\b");
    }
    printf("}\n");
}
void find(linkedlist *p, char ch)
{
    node *current = p->head->next;
    int position = 1;
    while (current != NULL)
    {
        if (ch == current->data)
        {
            printf("已找到%c 在%d位置\n", current->data, position);
            return;
        }
        current = current->next;
        position++;
    }

    printf("未找到\n");
}
void clear(linkedlist *p)
{
    node *current = p->head->next;
    while (current != NULL)
    {
        node *next = current->next;
        free(current);
        current = next;
    }
    p->head->next = NULL;
}
void delete(linkedlist *p, char ch)
{
    node *pre = p->head;           // 前一个结点
    node *current = p->head->next; // 当前结点
    while (current != NULL)
    {
        if (current->data == ch)
        {
            pre->next = current->next;
            free(current);
            printf("删除元素 '%c' 成功\n", ch);
            return;
        }
        pre = current;
        current = current->next;
        printf("没有找到元素 '%c'\n", ch);
    }
}

node *revers(node *p)
{
    node *crr = p;
    node *prev = NULL;
    node *next = NULL;
    while (crr)
    {
        next = crr->next;
        crr->next = prev;
        prev = crr;
        crr = next;
    }
    return prev;
}
// 快慢指针 找中点
node *mid(node *p)
{
    Node *slow = p;
    Node *fast = p->next;

    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next; // 走俩步
    }
    return slow;
}
void is(linkedlist *p)
{

    // 边界条件检查
    if (p->head->next == NULL || p->head->next->next == NULL)
    {
        printf("是回文\n");
        return;
    }

    int is = 1;
    node *mnode = mid(p->head->next);
    node *after = revers(mnode->next);
    node *afterCopy = after; // 保存翻转后的指针用于恢复
    node *before = p->head->next;

    while (after != NULL)
    {
        if (before->data != after->data)
        {
            is = 0;
            break;
        }
        before = before->next;
        after = after->next;
    }

    // 恢复原链表结构
    mnode->next = revers(afterCopy);

    if (is)
    {
        printf("是回文\n");
    }
    else
    {
        printf("不是回文\n");
    }
}
linkedlist droad(linkedlist *p,linkedlist *p1)
{
    linkedlist list1;


}