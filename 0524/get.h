#ifndef GET_H
#define GET_H

/**
 * @brief 从标准输入读取字符串
 * @param p 字符数组指针，用于存储读取的字符串
 * @param len 字符串当前长度
 * @return 读取的字符串长度（不含换行符）
 * @note 读取到换行符时自动结束，并将换行符替换为'\0'
 */
int get(char *p, int len);

#endif
