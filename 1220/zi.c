#include <stdio.h>
#include <wchar.h>
#include <locale.h>
int main()
{
    setlocale(LC_ALL, "zh_CN.UTF-8");
    wchar_t zi[] = {L'零', L'壹', L'贰', L'叁', L'肆', L'伍', L'陆', L'柒', L'捌', L'玖'};
    int money,i=0;   int t[10] = {0}; 
    wprintf(L"请输入金额: ");
    scanf("%d", &money);

    while (money > 0)
    {
        int b = money % 10;//b是倒序输入的
        t[i++] = b; //记入b的值  
        money /= 10;
    }
    for (i = 9; i >= 0; i--) //倒序输出b
    {
        if (t[i] != 0)
        {
            wprintf(L"%lc", zi[t[i]]);
        }
    }

    return 0;
}