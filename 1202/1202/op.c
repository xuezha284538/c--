#include <stdio.h>
#include <stdbool.h>
/// @brief op.c
/// @return
/** 关系运算,逻辑运算
 * 函数:函数体 ,函数名,返回值类型,参数列表
 *
 *
 *
 */

int main()
{
printf("请输入三个整数: ");
  int x,y,z,max,min,mid;
  scanf("%d %d %d",&x, &y, &z);
if (x>y)
  {
    max= x>z ? x : z;
    min= y<z ? y : z;
  }
  else
  {
    max= y>z ? y : z;
    min= x<z ? x : z;
  }
  mid= x + y + z - max - min;
  printf("max=%d\n",max);
  printf("mid=%d\n",mid);   
  printf("min=%d\n",min);
  
  //--------------***********------------------************----------------
  // printf("请输入一个整数: ");
  // int x,y,z,max;
  // scanf("%d %d %d",&x, &y, &z);
  // if (x>y)
  // {
  //   max= x>z ? x : z;
  // }
  // else
  // {
  //   max= y>z ? y : z;
  // }
  // printf("max=%d\n",max);




  //------*********--------------*************-----------------
  //? : : 三元运算符
  // %d, %c, %f, %s, %u, %p
  // int n;
  // printf("请输入一个整数: ");
  // scanf("%d", &n);printf("%d 是 %s\n", n, (n % 2 == 1) ? "奇数" : "偶数");

  //----------------------------**********
  // printf("请输入一个整数: ");
  // int n;

  // scanf("%d", &n);

  // if (1 == n % 2)
  // {
  //   printf("输入的是奇数 over\n");
  // }

  // else
  // {
  //   printf("输入的是偶数 over\n");
  // }

  //***************************---------------------*************************-------------
  //   int a = 1, b = 2;
  //   scanf("(%d)", &a);
  //   printf("%d\n", a == b); // 关系运算,逻辑运算
  //   printf("%d\n", a <= b); // 关系运算,逻辑运算
  //   printf("%d\n", a >= b); // 关系运算,逻辑运算
  //   printf("%d\n", a != b); // 关系运算,逻辑运算
  //   printf("%d\n", a < b);  // 关系运算,逻辑运算
  //   printf("%d\n", a > b);  // 关系运算,逻辑运算
  // bool b1=;
  // printf("%d\n",b1);

  return 0;
}
