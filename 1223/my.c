#include <stdio.h>
#include <math.h>
//声明 定义函数 参数:形式参数
void print_arr(int arr[], int size)
 {
    for(int i=0; i<size ; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int su(int n)
{
    int c=0;
    for(int i=2; i*i<=n; i++)
    {
        if (n%i==0)
    {
        //printf("%d不是质数 ", n);
        c++;
        break;
    }
    
    }
   if (c==0)
   {
        // printf("%d是质数 ", n);
        return 1;
   }
    return 0;
}
int fact(int n)
{ 
   if (n==1) //返回终止条件 收敛
   {
         return 1;
   }  
    return n*fact(n-1); 
   
   
}
