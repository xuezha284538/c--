#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int SeqSearch1(int R[],int n,int k)
{  int i=0;
   R[n]=k;
   while (R[i]!=k)	//从表头往后找
      i++;
   if (i==n)			          //未找到返回0
      return 0;
   else 
     return i+1;		          //找到返回逻辑序号i+1
}
int main(int argc, char const* argv[])
{
    int a[11]={0,1,2,3,4,5,6,7,8,9};
     printf("%d",SeqSearch1(a,10,7));
    
}