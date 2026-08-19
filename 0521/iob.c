// iob.c 对于二进制文件 写一个整形

#include <stdio.h>
#include <stdlib.h>
int main()
{
  FILE *fp = fopen("a.data", "wb"); // 以二进制方式写入
  // rb,wb ...
  if (!fp)
  {
    perror("无法打开文件");
    return EXIT_FAILURE;
  }
  // 逻辑 遍历
  int n = 23; // 4
  fwrite(&n, sizeof(int), 1, fp);

  // 关闭
  fclose(fp);
  return EXIT_SUCCESS;
}