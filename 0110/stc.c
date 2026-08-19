#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct birthday
{
    int year;
    int mouth;
    int day;
    // char b[9];
};

struct xuezha // 组合数据类型 封装了多个成员
{
    // 名字 name
    char name[16];
    // 成绩
    int score;
    // 性别
    char sex;
    struct birthday date;
};

void show(struct xuezha *a)
{
    printf("%s\t%d\t%c\t%02d-%02d-%02d\n", a->name, a->score, a->sex,a->date.year,a->date.mouth,a->date.day);
}

void up_score(struct xuezha *p, int a)
{
    p->score = a;
}

void up_sex(struct xuezha *p, char a)
{
    p->sex = a;
}
void up_name(struct xuezha *p, char a[12])
{
    strcpy(p->name, a);
}
void up_all(struct xuezha *p, char a[12], int b, char c)
{
    p->score = b;
    p->sex = c;
    strcpy(p->name, a);
}

int main(int argc, char const *argv[])
{
    struct xuezha s1 = {"liu", 89, 'm', 2002, 1, 5};
    struct xuezha s3;
    struct xuezha s2 = {.name="man",.score=88,.sex='a',.date.year=2001,.date.mouth=2,.date.day=5};
    struct xuezha *s4 = &s1;
    // s3.score = 99;
    // s3.sex = 'f';
    show(&s1);
    up_score(&s3, 123);
    up_name(&s3, "jiliao");
    up_sex(&s3, 'm');
    show(&s2);
    up_all(&s3, "bob", 66, 'f');
    show(&s3);
    return 0;
}