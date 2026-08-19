#include <stdio.h>   // 引入标准输入输出头文件，用于printf、scanf输入输出
#include <string.h>  // 引入字符串处理头文件，用于strlen/strcpy/memset/strcmp等字符串操作

// 函数功能：比较两个纯数字字符串的大小 (专门用于大数比较，无溢出风险)
// 参数说明：a-第一个数字字符串  b-第二个数字字符串
// 返回值：  1 代表a的数值大于b； -1 代表a的数值小于b； 0 代表两数相等
int cmp(char *a, char *b)
{
    int la = strlen(a), lb = strlen(b); // 获取两个字符串的长度
    if (la > lb)                        // 规则1：数字位数多的，数值一定更大
        return 1;
    if (la < lb)
        return -1;
    while (*a && *b)                    // 规则2：位数相等时，从左到右逐位比较字符大小
    {
        if (*a > *b)                    // 当前位a更大，整体a更大
            return 1;
        if (*a < *b)                    // 当前位a更小，整体a更小
            return -1;
        a++, b++;                       // 指针后移，比较下一位
    }
    return 0;                           // 所有位都相等，两数相等
}

// 函数功能：交换两个字符串的内容
// 参数说明：a-第一个字符串  b-第二个字符串
// 应用场景：减法时，小数减大数，需要交换两个数，保证大数减小数，最后补负号
void swap(char *a, char *b)
{
    char t[100];        // 定义临时字符串数组作为中转
    strcpy(t, a);       // 第一步：把a的内容拷贝到临时数组t
    strcpy(a, b);       // 第二步：把b的内容拷贝到a
    strcpy(b, t);       // 第三步：把t的内容拷贝到b，完成交换
}

// 函数功能：整型数组的加法运算 (核心复用函数，乘法的循环加法依赖此函数)
// 参数说明：res-存储最终累加结果的逆序数组  tmp-需要累加的临时逆序数组  len-需要计算的最大位数
// 返回值：  相加后结果数组的有效数字长度，逆序存储规则【res[0]=个位 res[1]=十位 res[2]=百位】
// 运算规则：逆序相加，带进位处理，和竖式加法从个位加起的逻辑一致
int arr_add(int *res, int *tmp, int len)
{
    int carry = 0, i;   // carry:进位标记，初始为0；i:循环下标
    for (i = 0; i < len; i++)
    {
        res[i] = res[i] + tmp[i] + carry; // 当前位 = 结果位+临时位+上一位的进位
        carry = res[i] / 10;               // 计算进位：和>=10则进位为1，否则为0
        res[i] %= 10;                      // 计算当前位的最终值：取和的个位数字
    }
    if (carry)                             // 处理最后一位的进位，比如999+1=1000，最后要补1
        res[i++] = carry;
    return i;                              // 返回相加后的有效位数
}

// 函数功能：比较两个逆序存储的整型大数数组的大小 (除法专用，判断被除数是否够减除数)
// 参数说明：a-数组a  len_a-数组a的有效长度  b-数组b  len_b-数组b的有效长度
// 返回值：  1 代表a>b； -1 代表a<b； 0 代表a==b
// 存储规则：数组下标0=个位，下标1=十位，下标越大位数越高
int arr_cmp(int *a, int len_a, int *b, int len_b)
{
    if (len_a > len_b)                // 规则1：位数多的数值大
        return 1;
    if (len_a < len_b)
        return -1;
    for (int i = len_a - 1; i >= 0; i--) // 规则2：位数相等，从最高位到最低位逐位比较
    {
        if (a[i] > b[i])
            return 1;
        if (a[i] < b[i])
            return -1;
    }
    return 0;                         // 所有位相等，两数相等
}

// 函数功能：整型数组的减法运算 (大数减小数，保证无负数，带借位) 除法的循环减法依赖此函数
// 参数说明：a-被减数逆序数组  len_a-被减数有效长度  b-减数逆序数组  len_b-减数有效长度
// 返回值：  相减后，被减数数组a的有效数字长度
// 前置条件：调用此函数时，必须保证 a的数值 >= b的数值
// 运算规则：逆序相减，带借位处理，和竖式减法从个位减起的逻辑一致
int arr_sub(int *a, int len_a, int *b, int len_b)
{
    int borrow = 0;     // borrow:借位标记，初始为0，需要借位时置1
    for (int i = 0; i < len_a; i++)
    {
        int n1 = a[i] - borrow;        // 当前位被减数 = 原值 - 上一位的借位
        int n2 = i < len_b ? b[i] : 0; // 当前位减数，减数位数不够时，补0
        borrow = 0;                    // 每次计算先清空借位标记
        if (n1 < n2)                   // 当前位被减数 < 减数，需要向高位借1当10
        {
            n1 += 10;                  // 借位后，当前位数值+10
            borrow = 1;                // 标记借位，高位需要减1
        }
        a[i] = n1 - n2;                // 计算当前位的最终差值，存入数组
    }
    // 去掉数组末尾的无效0（高位0），比如相减结果是0099，要变成99，返回有效长度2
    while (len_a > 1 && a[len_a - 1] == 0)
        len_a--;
    return len_a;                      // 返回相减后的有效位数
}

// 主函数：程序入口，整合加减乘除四则运算的完整逻辑
int main()
{
    char s1[100], s2[100];             // 定义两个字符数组，存储输入的两个数字字符串（最大支持99位数字）
    int res[201] = {0}, tmp[201] = {0};// res：存储加减乘的最终结果；tmp：存储运算中的临时结果；初始值全0，最大支持200位结果，防止溢出
    int len1, len2, maxlen, i, carry, borrow; // 辅助变量：长度/循环下标/进位/借位标记

    // 1. 输入处理：获取用户输入的两个纯数字字符串
    printf("输入第一个数字：");
    scanf("%s", s1);
    printf("输入第二个数字：");
    scanf("%s", s2);
    len1 = strlen(s1), len2 = strlen(s2); // 获取两个输入数字的长度

    // ===================== 1. 加法运算【核心：逆序存储计算 + 逆序打印输出】 =====================
    maxlen = len1 > len2 ? len1 : len2;   // 加法需要计算的最大位数 = 较长数字的位数
    carry = 0;                            // 初始化加法进位标记为0
    memset(res, 0, sizeof(res));          // 把结果数组res全部清零，消除脏数据影响
    for (i = 0; i < maxlen; i++)
    {
        // 逆序取数：字符串的最后一位是数字的个位，倒数第二位是十位，以此类推
        int n1 = i < len1 ? s1[len1 - 1 - i] - '0' : 0; // 取第一个数的第i位，位数不够补0
        int n2 = i < len2 ? s2[len2 - 1 - i] - '0' : 0; // 取第二个数的第i位，位数不够补0
        res[i] = n1 + n2 + carry;                       // 当前位求和 = 加数1 + 加数2 + 进位
        carry = res[i] / 10;                            // 计算进位
        res[i] %= 10;                                   // 计算当前位最终值
    }
    if (carry)                                          // 处理最后一位的进位
        res[maxlen++] = carry;
    // 打印加法结果：逆序打印 = 正序输出，数组最后一位是最高位，第一位是个位
    printf("加法：%s + %s = ", s1, s2);
    for (i = maxlen - 1; i >= 0; i--)
        printf("%d", res[i]);
    printf("\n");

    // ===================== 2. 减法运算【核心：逆序存储计算 + 逆序打印输出】 =====================
    borrow = 0;                            // 初始化减法借位标记为0
    memset(res, 0, sizeof(res));          // 清零结果数组，复用存储减法结果
    printf("减法：%s - %s = ", s1, s2);
    int f = 0;                             // 负号标记：0=无负号 1=有负号
    char a[100], b[100];                   // 定义临时字符串，防止修改原输入字符串
    strcpy(a, s1);
    strcpy(b, s2);
    if (cmp(a, b) < 0)                     // 判断：如果被减数 < 减数
    {
        f = 1;                             // 标记需要输出负号
        swap(a, b);                        // 交换两个数，保证 大数 - 小数，避免负数计算
    }
    len1 = strlen(a), len2 = strlen(b);    // 更新交换后的数字长度
    for (i = 0; i < len1; i++)
    {
        int n1 = a[len1 - 1 - i] - '0' - borrow; // 被减数当前位 - 借位
        int n2 = i < len2 ? b[len2 - 1 - i] - '0' : 0; // 减数当前位，位数不够补0
        borrow = 0;
        if (n1 < n2)                       // 当前位不够减，借位处理
        {
            n1 += 10;
            borrow = 1;
        }
        res[i] = n1 - n2;                  // 存储当前位的差值
    }
    if (f)                                 // 如果标记了负号，先打印负号
        printf("-");
    // 跳过结果数组的高位无效0，比如结果是0099，只打印99；如果全0则打印0
    for (i = len1 - 1; i >= 0 && res[i] == 0; i--);
    i < 0 ? printf("0") : 0;               // 特殊情况：两数相等，结果为0
    for (; i >= 0; i--)                    // 逆序打印减法结果
        printf("%d", res[i]);
    printf("\n");

    // ===================== 3. 乘法运算【核心：循环加法 + 补零对齐 】 =====================
    memset(res, 0, sizeof(res));          // 清零结果数组，复用存储乘法结果
    printf("乘法：%s × %s = ", s1, s2);
    strcpy(a, s1);
    strcpy(b, s2);
    len1 = strlen(a), len2 = strlen(b);
    // 乘法规则：乘数的第i位 × 被乘数，结果需要补i个0（个位乘补0，十位乘补1个0，百位乘补2个0）
    for (i = 0; i < len2; i++)
    {
        memset(tmp, 0, sizeof(tmp));      // 清零临时数组，存储单次乘积结果
        carry = 0;                        // 初始化乘法进位标记
        int mul = b[len2 - 1 - i] - '0';  // 逆序取乘数的第i位（从个位开始）
        // 第一步：计算 被乘数 × 乘数的当前位，得到临时乘积结果
        for (int j = 0; j < len1; j++)
        {
            int n = (a[len1 - 1 - j] - '0') * mul + carry; // 当前位乘积 + 进位
            tmp[j] = n % 10;                                // 存储当前位结果
            carry = n / 10;                                // 计算进位
        }
        if (carry)                                         // 处理最后一位的进位
            tmp[len1] = carry;
        // 第二步：补零对齐核心逻辑 - 数组下标偏移i位 = 补i个0，完美对齐竖式乘法
        int tmp2[201] = {0};
        for (int j = 0; j <= len1; j++)
            tmp2[j + i] = tmp[j];
        // 第三步：循环加法，把对齐后的临时结果累加到最终结果数组
        arr_add(res, tmp2, len1 + i + 1);
    }
    // 跳过高位无效0，打印乘法结果，无前置零
    maxlen = len1 + len2;
    for (i = maxlen; i >= 0 && res[i] == 0; i--);
    i < 0 ? printf("0") : 0;
    for (; i >= 0; i--)
        printf("%d", res[i]);
    printf("\n");

    // ===================== 4. 除法运算【核心：循环减法(试商法) 乘法的逆运算】 =====================
    memset(res, 0, sizeof(res));          // 清零数组，消除前面运算的脏数据
    memset(tmp, 0, sizeof(tmp));
    printf("除法：%s ÷ %s = ", s1, s2);
    strcpy(a, s1);
    strcpy(b, s2);
    len1 = strlen(a), len2 = strlen(b);
    int dividend[201] = {0}, divisor[201] = {0}; // dividend-被除数数组  divisor-除数数组 均为逆序存储
    int quotient[201] = {0};                     // quotient-商数组，逆序存储，初始全0
    int len_div = len1, len_dvs = len2;           // 被除数/除数的有效长度

    // 除法必备容错：数学规则，除数不能为0，否则无意义，直接提示并退出
    if (cmp(b, "0") == 0)
    {
        printf("除数不能为0！\n");
        return 0;
    }
    // 被除数：数字字符串 转 逆序整型数组
    for (i = 0; i < len1; i++)
        dividend[i] = a[len1 - 1 - i] - '0';
    // 除数：数字字符串 转 逆序整型数组
    for (i = 0; i < len2; i++)
        divisor[i] = b[len2 - 1 - i] - '0';

    // 除法核心逻辑：试商法 = 循环减法 【乘法是循环加法，除法是循环减法，完美对应】
    // 规则：被除数 连续减 除数，能减多少次，商就是多少；减不动的剩余值就是余数
    while (arr_cmp(dividend, len_div, divisor, len_dvs) >= 0)
    {
        len_div = arr_sub(dividend, len_div, divisor, len_dvs); // 被除数 = 被除数 - 除数
        // 商+1 并处理进位：商是逆序存储，个位在数组0位，满10进1
        carry = 1;
        i = 0;
        while (carry && i < 200)
        {
            quotient[i] += carry;
            carry = quotient[i] / 10;
            quotient[i] %= 10;
            i++;
        }
    }

    // 打印商：跳过高位无效0，无前置零，逆序打印为正序
    int len_q = 200;
    for (i = len_q; i >= 0 && quotient[i] == 0; i--);
    if (i < 0)
        printf("0"); // 被除数 < 除数时，商为0
    else
        for (; i >= 0; i--)
            printf("%d", quotient[i]);

    // 打印余数：减法结束后，被除数数组中剩余的数值就是余数，逆序打印
    printf(" ...... 余数：");
    for (i = len_div - 1; i >= 0 && dividend[i] == 0; i--);
    if (i < 0)
        printf("0"); // 能整除时，余数为0
    else
        for (; i >= 0; i--)
            printf("%d", dividend[i]);
    printf("\n");

    return 0; // 程序正常结束
}