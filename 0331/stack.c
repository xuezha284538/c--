#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define max 9
// 数据
typedef struct stack
{
    char data[max]; // 顺序表
    int top;        // 栈顶指针

} stack;

// 操作
void init(stack *s);
void push(stack *s, char);
char pop(stack *s);
int top(stack *s);
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
    push(&s, 'd');
    push(&s, 'c');
    push(&s, 'b');
    pop(&s);
    // printf("%c",s.data[0]);
    printf("%c\n",top(&s));
    show(&s);
}
void init(stack *s)
{
    s->top = -1;
}
void push(stack *s, char ch)
{
    if (s->top == max - 1)
    {
        return;
    }
    s->top++;
    s->data[s->top] = ch;
}
char pop(stack *s)
{
   if (s->top == max - 1)
    {
        return EXIT_FAILURE;
    }

    return  s->data[s->top--] = 0;
}
int top(stack *s)
{
    return s->data[s->top];
}

void clean(stack *s)
{
    s->top=-1;
}
bool empty(stack *s)
{
   return s->top==-1;
}
void show(stack *s)
{
    printf("{");
    for(int i=s->top;i>=0;i--)
    {
        printf("%c,",s->data[i]);
    }
    printf("}\b\n");

}

// 4+2-3+5*6*2-9/3-1
//输出:4,2,+,3,-,5,6,*,2,*,+,9,3,/,-,1,-
//4,2,+,3,-,5,6,*,2,*,+,9,3,/,-,1,-
//60-1=59
//栈:

//2+3*(5-2*2-1)-4*(3-1*2)-9=-11
//2,3,5,2,2,*,-,1,-,*,+,4,3,1,2,*,-,*,-,9,-
//2,3,5,2,2,
//实列:链式栈 -头插法