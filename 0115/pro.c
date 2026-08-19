#include <stdio.h>
#include <string.h>
#define MAX 150
#define SHORT_LEN 8                       // 短码长度8位
#define BASE 62                           // 62进制
#define SHORT_URL_PREFIX "http://xxx.cn/" // 短网址前缀

// ============================================
// 全局变量区
// ============================================

// 全局序号：每生成一个短网址，序号+1（保证唯一性）
int url_seq = 1;

// ============================================
// 数据结构定义
// ============================================

// 短网址信息结构体
// 存储每条短网址的完整信息
typedef struct
{
    char short_w[23];  // 固定长度：前缀14 + 短码8 + 结束符1 = 23
    char long_w[2048]; // 长网址（URL最大长度2048字节）
    int count;         // 已解析次数（访问统计）
    int max_count;     // 有效次数（-1=无限次，其他=具体次数限制）
} web;

// 短网址存储数组（内存存储）
web short_map[MAX];

// 当前已存储的短网址数量
int url_count = 0;

// ============================================
// 常量定义
// ============================================

// 62进制字符集：0-9 → a-z → A-Z（顺序固定，用于编码）
const char charset[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

// ============================================
// 核心算法函数
// ============================================

/**
 * 62进制编码函数
 *
 * 功能：将整数序号转换为62进制短码
 * 算法：除62取余法，将余数映射到字符集
 *
 * @param seq       输入：整数序号（从1开始）
 * @param short_code 输出：8位62进制短码字符串
 *
 * 示例：
 *   seq=1   → "00000001"
 *   seq=61  → "0000000z"
 *   seq=62  → "00000010"
 *   seq=3844→ "00000100"
 */
void encode_62(int seq, char *short_code)
{
    int i = 0;
    char temp[SHORT_LEN + 1] = {0};

    // 第一步：除62取余，得到逆序的短码
    while (seq > 0 && i < SHORT_LEN)
    {
        int remainder = seq % BASE;     // 取余数（0-61）
        temp[i++] = charset[remainder]; // 映射到对应字符
        seq = seq / BASE;               // 整除62，继续下一轮
    }

    // 第二步：不足8位的前面补0
    while (i < SHORT_LEN)
    {
        temp[i++] = '0';
    }

    // 第三步：逆序排列，得到正序短码
    for (int j = 0; j < SHORT_LEN; j++)
    {
        short_code[j] = temp[SHORT_LEN - 1 - j];
    }
    short_code[SHORT_LEN] = '\0';
}

// ============================================
// 业务逻辑函数
// ============================================

/**
 * 生成完整短网址的核心函数
 *
 * 功能：
 *   1. 检查长网址是否已存在（防重复）
 *   2. 检查存储容量
 *   3. 生成62进制短码
 *   4. 拼接前缀生成完整短网址
 *   5. 存入结构体数组
 *
 * @param long_url   输入：原始长网址
 * @param max_count  输入：有效次数（-1表示无限）
 * @param short_url  输出：生成的短网址
 *
 * @return  1=生成成功, 0=已存在, -1=存储已满
 */
int generate_short_url(const char *long_url, int max_count, char *short_url)
{
    // 1. 检查长网址是否已生成过（防重复）
    for (int i = 0; i < url_count; i++)
    {
        if (strcmp(short_map[i].long_w, long_url) == 0)
        {
            strcpy(short_url, short_map[i].short_w);
            return 0; // 0=已存在，返回已生成的短网址
        }
    }

    // 2. 检查数组是否已满
    if (url_count >= MAX)
    {
        return -1; // -1=存储已满
    }

    // 3. 生成62进制短码
    char short_code[SHORT_LEN + 1];
    encode_62(url_seq, short_code);

    // 4. 拼接前缀+短码，生成完整短网址
    strcpy(short_url, SHORT_URL_PREFIX);
    strcat(short_url, short_code);

    // 5. 存入结构体数组
    strcpy(short_map[url_count].short_w, short_url);
    strcpy(short_map[url_count].long_w, long_url);
    short_map[url_count].count = 0;             // 初始解析次数为0
    short_map[url_count].max_count = max_count; // 设置有效次数

    // 6. 序号和计数自增
    url_seq++;
    url_count++;

    return 1; // 1=生成成功
}

/**
 * 辅助函数：提取短码并验证格式
 *
 * 验证规则：
 *   - 总长度必须为22位（14位前缀+8位短码）
 *   - 前缀必须匹配 SHORT_URL_PREFIX
 *
 * @param short_url   输入：完整短网址
 * @param short_code  输出：提取的8位短码
 *
 * @return  1=格式正确, 0=格式错误
 */
int extract_short_code(const char *short_url, char *short_code)
{
    int url_len = strlen(short_url);
    // 完整短网址长度：14位前缀+8位短码=22位（不含结束符）
    if (url_len != 22)
    {
        return 0;
    }

    // 验证前缀
    char prefix[15] = {0};
    strncpy(prefix, short_url, 14);
    if (strcmp(prefix, SHORT_URL_PREFIX) != 0)
    {
        return 0;
    }

    // 提取短码
    strncpy(short_code, short_url + 14, 8);
    short_code[8] = '\0';
    return 1;
}

/**
 * 解析短网址核心函数
 *
 * 功能：
 *   1. 验证短网址格式
 *   2. 查找对应的长网址
 *   3. 检查有效次数限制
 *   4. 更新解析次数统计
 *
 * @param input_short_url  输入：要解析的短网址
 * @param long_url         输出：对应的长网址
 * @param count            输出：当前解析后的次数
 * @param max_count        输出：有效次数限制
 *
 * @return  1=解析成功, 0=未找到, -1=格式错误, -2=超过有效次数
 */
int decode_short_url(const char *input_short_url, char *long_url, int *count, int *max_count)
{
    // 1. 验证短网址格式
    char short_code[SHORT_LEN + 1];
    if (extract_short_code(input_short_url, short_code) == 0)
    {
        return -1; // -1=格式错误
    }

    // 2. 遍历数组查找短网址
    for (int i = 0; i < url_count; i++)
    {
        if (strcmp(short_map[i].short_w, input_short_url) == 0)
        {
            // 3. 检查是否超过有效次数
            // max_count=-1表示无限次，其他值表示具体限制
            if (short_map[i].max_count != -1 && short_map[i].count >= short_map[i].max_count)
            {
                return -2; // -2=超过有效次数，无法访问
            }

            // 4. 解析成功，更新解析次数
            strcpy(long_url, short_map[i].long_w);
            *count = short_map[i].count + 1; // 次数+1
            *max_count = short_map[i].max_count;
            short_map[i].count = *count; // 更新存储的计数

            return 1; // 1=解析成功
        }
    }

    return 0; // 0=未找到短网址
}

/**
 * 展示所有短地址
 *
 * 功能：格式化输出所有短网址记录
 * 显示内容：序号、短网址、已解析次数、有效次数、剩余次数、原始长网址
 */
void show_all_short_urls()
{
    // 1. 先判断是否有记录
    if (url_count == 0)
    {
        printf("\n=====所有短网址记录=====\n");
        printf("⚠️  暂无任何短网址记录！\n");
        printf("=======================\n");
        return;
    }

    // 2. 格式化输出表头
    printf("\n=====所有短网址记录（共%d条）=====\n", url_count);
    // 表头对齐（用\t和空格调整，保证可读性）
    printf("%-4s | %-25s | %-10s | %-10s | %-10s | %s\n",
           "序号", "短网址", "已解析", "有效次数", "剩余次数", "原始长网址");
    printf("-------------------------------------------------------------------------------------------------------------------------\n");

    // 3. 遍历结构体数组，逐行输出每条记录
    for (int i = 0; i < url_count; i++)
    {
        // 定义临时变量存储状态文本
        char max_count_str[20];    // 有效次数文本（"3次"或"无限次"）
        char remain_count_str[20]; // 剩余次数文本（"2次"或"无限次"或"已失效"）

        // 处理有效次数的显示
        if (short_map[i].max_count == -1)
        {
            strcpy(max_count_str, "无限次");
            strcpy(remain_count_str, "无限次");
        }
        else
        {
            // 计算剩余次数
            int remain = short_map[i].max_count - short_map[i].count;
            sprintf(max_count_str, "%d次", short_map[i].max_count);
            if (remain <= 0)
            {
                strcpy(remain_count_str, "已失效");
            }
            else
            {
                sprintf(remain_count_str, "%d次", remain);
            }
        }

        // 输出单条记录（对齐排版）
        printf("%-4d | %-22s | %-7d | %-9s | %-9s | %s\n",
               i + 1,                // 序号（从1开始显示）
               short_map[i].short_w, // 短网址
               short_map[i].count,   // 已解析次数
               max_count_str,        // 有效次数
               remain_count_str,     // 剩余次数
               short_map[i].long_w); // 原始长网址
    }
    printf("-------------------------------------------------------------------------------------------------------------------------\n");
}

// ============================================
// 主函数：命令行交互界面
// ============================================

/**
 * 主函数
 *
 * 提供命令行菜单：
 *   1. 生成短网址
 *   2. 解析短网址
 *   3. 查看所有短网址
 *   4. 退出程序
 */
int main()
{
    int choice;
    char input_long[2048];
    char short_url_result[23];

    // 主循环：持续显示菜单直到用户选择退出
    while (1)
    {
        // 显示菜单
        printf("\n~~~~~短网址服务~~~~~\n");
        printf("1.生成短网址\n");
        printf("2.解析短网址\n");
        printf("3.查看所有短网址\n");
        printf("4.退出\n");
        printf("~~~~~~~~~~~~~~~~~~~~~\n");
        printf("请选择: ");
        scanf("%d", &choice);
        // 清空输入缓冲区，防止影响后续的fgets
        while (getchar() != '\n')
            ;

        // 根据选择执行对应功能
        switch (choice)
        {
        case 1:
            // ===== 生成短网址 =====
            printf("请输入长网址：");
            fgets(input_long, sizeof(input_long), stdin);
            input_long[strcspn(input_long, "\n")] = '\0'; // 去除末尾换行符

            // 输入有效次数
            printf("请输入有效次数(-1为无限次): ");
            int max_count;
            scanf("%d", &max_count);
            while (getchar() != '\n')
                ; // 清空缓冲区

            // 调用生成函数
            int result = generate_short_url(input_long, max_count, short_url_result);
            if (result == 1)
            {
                printf("短网址生成成功：%s\n", short_url_result);
            }
            else if (result == 0)
            {
                printf("该长网址已生成过短网址：%s\n", short_url_result);
            }
            else
            {
                printf("生成失败：短网址存储已满！\n");
            }
            break;

        case 2:
        {
            // ===== 解析短网址 =====
            char input_short_url[23];
            char long_url_result[2048];
            int parse_count; // 已解析次数
            int max_count;   // 有效次数

            printf("请输入要解析的短网址：");
            fgets(input_short_url, sizeof(input_short_url), stdin);
            input_short_url[strcspn(input_short_url, "\n")] = '\0';

            // 调用解析函数
            int decode_result = decode_short_url(input_short_url, long_url_result, &parse_count, &max_count);
            if (decode_result == 1)
            {
                // 解析成功
                printf("\n-----解析结果-----\n");
                printf("原长网址：%s\n", long_url_result);
                printf("本次解析后次数：%d\n", parse_count);
                if (max_count == -1)
                {
                    printf("有效次数：无限次\n");
                }
                else
                {
                    printf("有效次数：%d次（剩余%d次）\n", max_count, max_count - parse_count);
                }
                printf("---\n");
            }
            else if (decode_result == 0)
            {
                printf("解析失败：未找到该短网址！\n");
            }
            else if (decode_result == -1)
            {
                printf("解析失败：短网址格式错误（示例：http://xxx.cn/00000001）！\n");
            }
            else if (decode_result == -2)
            {
                printf("解析失败：已达到最大有效次数，无法解析！\n");
            }
        }
        break;

        case 3:
            // ===== 查看所有短网址 =====
            show_all_short_urls();
            break;

        case 4:
            // ===== 退出程序 =====
            printf("程序退出！\n");
            return 0;

        default:
            // 无效选择
            printf("无效指令，请重新选择！\n");
        }
    }
    return 0;
}
