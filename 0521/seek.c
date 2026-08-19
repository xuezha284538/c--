#include <stdio.h>
#include <stdlib.h>
int main(int argc, char const *argv[])
{
   FILE *fp = fopen(argv[1], "r");
   if (!fp)
   {
      perror("无法打开文件");
      return 1;
   }
   // 光标从文件末尾跳过0个字节
   fseek(fp, 0, SEEK_END); // 参数 第一个fp文件指针 第二个 跳过多少字节 第三个 光标位置  就是从光标位置跳过多少字节
   long size = ftell(fp);
   // 回到文件开头
   rewind(fp);

   char *buf = malloc(size);
   fread(buf, 1, size, fp);
   printf("%s", buf);

   //  int ch;
   //   ch=fgetc(fp);
   //  printf("%c\n",ch);

   // fseek(fp,4,SEEK_SET);//随机读取
   //  ch=fgetc(fp);
   //  printf("%c\n",ch);

   //  rewind(fp);//回到开头
   //  ch=fgetc(fp);
   //  printf("%c\n",ch);

   // 跳转光标到文件末尾
   //  fseek(fp,0,SEEK_END);//参数 第一个fp文件指针 第二个 跳过多少字节 第三个 光标位置  就是从光标位置跳过多少字节
   //  long size =ftell(fp);
   //  printf("size:%ld byte\n",size);
   return 0;
}