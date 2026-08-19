#include <stdio.h>

#include <string.h>
int main(int argc, char const *argv[])
{
    FILE *fp = fopen("a.csv", "w");
    // format printf to file
    fprintf(fp, "%s,%d\n", "alice", 20);
    fprintf(fp, "%s,%d\n", "s", 13);
    fprintf(fp, "%s,%d\n", "x", 02);
    fprintf(stdout, "%s,%d\n", "w", 33);

    // format printf to string
    // sprintf
    char text[32];
    char *name = "bob";
    char *tel = "1313235115";
    int age = 21;
    sprintf(text, "姓名:%s 手机号:%s 年龄%d", name, tel, age); // 参数 1.放入的字符串的数组,2格式,3 你要传入的数据 也就是字符串
    printf("%s", text);

    fclose(fp);
    return 0;
}