#include <stdio.h>

/**杨辉三角
 * 
 * 
 */
int main()
{
    // char zi[] = "零壹贰叁肆伍陆柒捌玖";
    // char *zi  ="零壹贰叁肆伍陆柒捌玖";
    // char *zi[10] = {"零", "壹", "贰", "叁", "肆", "伍", "陆", "柒", "捌", "玖"};

    // 方案一 :整数
    // 方案二 :字符串


    int m[9][9] = {0};
    for (int i = 0; i < 9; i++)
    {
        m[i][0] = 1;
        m[i][i] = 1;

        for (int j = 0; j < 9; j++)
        { wprintf(L"%lc", zi[b]);
            if (i >= 2 && j >= 1 && j < i)
            {
                m[i][j] = m[i - 1][j - 1] + m[i - 1][j];
            }

            if (m[i][j] != 0)
            {
                printf("%d\t", m[i][j]);
            }
            if (j % 9 == 8)
            {
                printf("\n");
            }
        }
    }
    return 0;
}