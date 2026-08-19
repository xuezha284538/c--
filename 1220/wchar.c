/** @addtogroup wchar
 * 宽字符
 * 
 * l10n 本地化支持
 * i18n 国际化支持
 * 
 *  */
 

#include <stdio.h>
#include <wchar.h>
#include <locale.h>

int main()
{
    setlocale(LC_ALL,"zh_CN.UTF-8"); //设置程序的本地化环境 语言语种 _地区.编码格式
    wchar_t ch=L'😀';
    wprintf(L"字符=%lc 编码值=%#x\n ",ch,ch);


return 0;

}