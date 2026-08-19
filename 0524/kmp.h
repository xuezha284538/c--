#ifndef KMP_H
#define KMP_H

/**
 * @brief 构建KMP算法的next数组
 * @param a next数组，用于存储部分匹配表
 * @param b 模式字符串
 * @param len 模式字符串长度
 * @note next[i]表示模式串前i+1个字符的最长公共前后缀长度
 */
void next1(int a[], char b[], int len);

/**
 * @brief KMP字符串匹配算法
 * @param a 主字符串
 * @param s 模式字符串
 * @param next next数组
 * @param s_len 模式字符串长度
 * @param a_len 主字符串长度
 * @return 匹配成功返回起始位置，失败返回-1
 * @note 时间复杂度O(n+m)，空间复杂度O(m)
 */
int kmp(char* a, char* s, int* next, int s_len, int a_len);

/**
 * @brief 从指定位置开始的KMP字符串匹配算法
 * @param a 主字符串
 * @param s 模式字符串
 * @param next next数组
 * @param s_len 模式字符串长度
 * @param a_len 主字符串长度
 * @param start 开始匹配的位置
 * @return 匹配成功返回起始位置，失败返回-1
 * @note 用于实现多次匹配功能
 */
int kmp_from(char* a, char* s, int* next, int s_len, int a_len, int start);

#endif