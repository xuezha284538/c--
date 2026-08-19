#ifndef BF_H
#define BF_H

/**
 * @brief 暴力匹配算法（Brute Force）
 * @param a 主字符串
 * @param s 模式字符串
 * @param a_len 主字符串长度
 * @param s_len 模式字符串长度
 * @return 匹配成功返回起始位置，失败返回-1
 */
int bf(char *a, char *s, int a_len, int s_len);

#endif
