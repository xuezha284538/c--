// iob3.c 读取

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct person
{
    char a[16];
    int age
} person;

int main(int argc, char const *argv[])
{
    FILE *fp = fopen("a.data", "rb"); // 以二进制方式写入
    // FILE *fp2=fopen("a.data","rb");
    // rb,wb ...
    if (!fp)
    {
        perror("无法打开文件");
        return EXIT_FAILURE;
    }
    // 逻辑 遍历
    int *n;
    person *p;

    fread(n, sizeof(int), 1, fp);
    fread(p, sizeof(person), 1, fp);

    printf("%d\n", *n);
    printf("%s,%d", p->a, p->age);
    // int a[12]={0,12,1};
    // fwrite(&n,sizeof(int),1,fp);
    // fwrite(&p1,sizeof(person),1,fp);
    // fwrite(&a,sizeof(a),sizeof(a)/sizeof(int),fp);

    // 关闭
    fclose(fp);
    return EXIT_SUCCESS;
}