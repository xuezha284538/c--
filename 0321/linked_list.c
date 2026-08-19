// 头插法 实现单链表
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
// 节点
typedef struct node // 最小单链表 头节点
{
    char data;  // 数据域
    int position; // 节点在链表中的位置
    struct node *next; // 指针域 后继

} node, Node, linkedlist;
void init(linkedlist *p);
void add(linkedlist *p, char ch);
void show(linkedlist *p);
void find(linkedlist *p, char ch);
void clear(linkedlist *p);
void delete(linkedlist *p, char ch);
void move(linkedlist *p, char ch, int position);
int get_position(linkedlist *p, char ch);

int main()
{
    linkedlist list;
    init(&list);
    add(&list, 'a');
    add(&list, '5');
    add(&list, '8');
    add(&list, '9');
    
    printf("初始链表: ");
    show(&list);
    
    // 查找元素
    find(&list, '8');
    find(&list, 'x');
    
    // 获取元素位置
    int pos = get_position(&list, '8');
    if (pos != 0)
        printf("元素 '8' 的位置是: %d\n", pos);
    else
        printf("元素 '8' 不存在\n");
    
    // 将元素 '5' 移动到位置 1（头部）
    move(&list, '5', 1);
    printf("移动后链表: ");
    show(&list);
    
    // 移动后再次获取元素位置
    pos = get_position(&list, '5');
    if (pos != 0)
        printf("元素 '5' 的位置是: %d\n", pos);
    else
        printf("元素 '5' 不存在\n");
    
    // 将元素 'a' 移动到位置 4（尾部）
    move(&list, 'a', 4);
    printf("移动后链表: ");
    show(&list);
    
    // 将元素 '8' 移动到位置 2
    move(&list, '8', 2);
    printf("移动后链表: ");
    show(&list);
    
    // 移动不存在的元素
    move(&list, 'x', 1);
    
    // 将已经在目标位置的元素移动
    move(&list, '5', 1);
    
    // 移动到超出范围的位置
    move(&list, '9', 10);
    
    // 删除元素
    delete(&list, '5');
    printf("删除后链表: ");
    show(&list);
    
    // 清空链表
    clear(&list);
    printf("清空后链表: ");
    show(&list);
    
    return 0;
}

void init(linkedlist *p)
{
    // 建立空表 :头节点的后继为空
    p->next = NULL;
}

void add(linkedlist *p, char ch)
{   //建立新结点
    node *new_node = malloc(sizeof(node));
    //赋值数据域
    new_node->data = ch;
    new_node->position = 1; // 头插法，新节点总是在位置1
    
    // 更新后续节点的位置
    node *current = p->next;
    while (current != NULL)
    {
        current->position++;
        current = current->next;
    }
    
    //新节点指向头的后继
    new_node->next = p->next;
    //头节点指向新节点
    p->next = new_node;
}
void show(linkedlist *p)
{   
    //首元结点
    node *node =p->next;
    printf("{");
    if (node != NULL)
    {
        while (node!=NULL)
        {
            printf("%c,",node->data);
            node =node->next;
        }
        
    }
    printf("}\b\n");
    
}

// 查找元素
void find(linkedlist *p, char ch)
{
    node *current = p->next;
    while (current != NULL)
    {
        if (current->data == ch)
        {
            printf("找到元素 '%c'，位置是 %d\n", ch, current->position);
            return;
        }
        current = current->next;
    }
    printf("没有找到元素 '%c'\n", ch);
}



// 清空链表
void clear(linkedlist *p)
{
    node *current = p->next;
    while (current != NULL)
    {
        node *next = current->next;
        free(current);
        current = next;
    }
    p->next = NULL;
}

// 删除元素
void delete(linkedlist *p, char ch)
{
    node *prev = p;
    node *current = p->next;
    
    while (current != NULL)
    {
        if (current->data == ch)
        {
            int deleted_pos = current->position;
            
            // 找到要删除的节点，修改指针跳过该节点
            prev->next = current->next;
            
            // 更新后续节点的位置
            node *update_node = prev->next;
            while (update_node != NULL)
            {
                update_node->position--;
                update_node = update_node->next;
            }
            
            free(current);
            printf("删除元素 '%c' 成功\n", ch);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("没有找到元素 '%c'\n", ch);
}

// 移动元素到指定位置
// 参数：p - 链表指针, ch - 要移动的元素, position - 目标位置(从1开始计数)
void move(linkedlist *p, char ch, int position)
{
    node *prev = p;              // 指向当前节点的前一个节点
    node *current = p->next;     // 当前检查的节点
    int current_pos;             // 当前节点的位置
    
    // 遍历链表查找目标元素
    while (current != NULL)
    {
        // 找到目标元素
        if (current->data == ch)
        {
            current_pos = current->position;
            
            // 如果元素已经在目标位置，不需要移动
            if (current_pos == position)
            {
                printf("元素 '%c' 已经在位置 %d\n", ch, position);
                return;
            }
            
            // 步骤1: 从原位置删除元素
            // 将前一个节点的next指针指向当前节点的下一个节点
            prev->next = current->next;
            
            // 更新受影响节点的位置
            node *update_node = prev->next;
            if (current_pos < position)
            {
                // 向前移动：从原位置+1到目标位置的节点位置减1
                while (update_node != NULL && update_node->position <= position)
                {
                    update_node->position--;
                    update_node = update_node->next;
                }
            }
            else
            {
                // 向后移动：从目标位置到原位置-1的节点位置加1
                node *temp = p->next;
                while (temp != NULL && temp->position < current_pos)
                {
                    if (temp->position >= position)
                    {
                        temp->position++;
                    }
                    temp = temp->next;
                }
            }
            
            // 步骤2: 找到目标位置的前一个节点
            node *target_prev = p;  // 从链表头开始找
            
            // 循环找到目标位置的前一个节点
            for (int i = 1; i < position; i++)
            {
                // 如果目标位置超出链表范围
                if (target_prev->next == NULL)
                {
                    printf("位置 %d 超出链表范围\n", position);
                    // 将元素插回原位置
                    prev->next = current;
                    return;
                }
                target_prev = target_prev->next;
            }
            
            // 步骤3: 将元素插入到目标位置
            current->next = target_prev->next;  // 当前节点指向目标位置的下一个节点
            target_prev->next = current;        // 目标位置的前一个节点指向当前节点
            
            // 更新当前节点的位置
            current->position = position;
            
            // 显示移动结果
            printf("元素 '%c' 已从位置 %d 移动到位置 %d\n", ch, current_pos, position);
            return;
        }
        // 继续遍历下一个节点
        prev = current;
        current = current->next;
    }
    
    // 遍历完链表没有找到目标元素
    printf("没有找到元素 '%c'\n", ch);
}

// 获取元素在链表中的位置
// 参数：p - 链表指针, ch - 要查找的元素
// 返回：元素的位置(从1开始计数)，如果元素不存在返回0
int get_position(linkedlist *p, char ch)
{
    node *current = p->next;
    
    while (current != NULL)
    {
        if (current->data == ch)
        {
            return current->position; // 直接返回节点的position字段
        }
        current = current->next;
    }
    
    return 0; // 元素不存在
}