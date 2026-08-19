// iob2.c 写结构体

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct person
{
    char a[16];
    int age
} person;

int main()
{
    person list[3] = {
        {"ali", 5},
        {"bo", 4},
        {"xx", 6}};

    FILE *fp = fopen("a.data", "wb"); // 以二进制方式写入
    FILE *fp2 = fopen("a.data", "rb");
    // rb,wb ...
    if (!fp)
    {
        perror("无法打开文件");
        return EXIT_FAILURE;
    }
    // 逻辑 遍历
    fwrite(list, sizeof(person), 3, fp);

    fclose(fp);
    person p[3];
    fread(p, sizeof(person), 3, fp2);

    for (int i = 0; i < 3; i++)
    {
        printf("%s,%d\n", p[i].a, p[i].age);
    }

    //  int n=23;//4
    //  person p1={"xuezha",16};
    //  person p2;
    //  p2.age=19;
    //  strcpy(p2.a,"bob");

    // int a[12]={0,12,1};
    //  fwrite(&n,sizeof(int),1,fp);
    //  fwrite(&p1,sizeof(person),1,fp);
    // fwrite(&a,sizeof(a),sizeof(a)/sizeof(int),fp);

    // 关闭

    fclose(fp2);
    return EXIT_SUCCESS;
}