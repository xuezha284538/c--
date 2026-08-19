#include <stdio.h>
#include <ctype.h>

int main()
{
    FILE *fp = fopen("io1.c", "r");
    if (fp == NULL)
        return 1;

    int s = 0, e = 0, p = 0; // 总行、空行、注释行
    char line[1024];

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        s++; // 总行数+1

        // 判断空行：全是空白字符
        int space = 1; // 是空行标志符
        for (int i = 0; line[i] != '\0'; i++)
        {
            if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
            {
                space = 0; // 有非空字符出现  不是空行跳出
                break;
            }
        }
        if (space) // 如果检测完毕标识符未改变说明是空行
        {
            e++; // 空行+1
            continue;
        }

        // 判断注释行
        // 包含 // 或 /* 或 */
        char *p_line = line;
        while (*p_line)
        {
            // 跳过字符串
            if (*p_line == '"')
            {
                p_line++;
                while (*p_line && *p_line != '"')
                    p_line++;
                if (*p_line)
                    p_line++;
                continue;
            }
            // 单行注释
            if (*p_line == '/' && *(p_line + 1) == '/')
            {
                p++;
                break;
            }
            // 多行注释开始或结束
            if (*p_line == '/' && *(p_line + 1) == '*')
            {
                p++;
                break;
            }
            p_line++;
        }
    }

    int c = s - e - p; // 代码行
    printf("总行数: %d\n", s);
    printf("空行数: %d\n", e);
    printf("注释行: %d\n", p);
    printf("代码行: %d\n", c);

    fclose(fp);
    return 0;
}