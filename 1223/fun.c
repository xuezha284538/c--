#include<stdio.h>
#include<math.h>
#include "my.h" //当前路径的my.h头文件
// 模块化:高内聚,低耦合
//kiss:keep it simple and stupid 单一任务原则
// //函数声明
// void print_arr(int arr[], int size);
// 斐波那契数列递归实现
// 原理：根据斐波那契数列定义 F(n) = F(n-1) + F(n-2)，递归调用直到基情况。
// 优点：代码简洁；缺点：重复计算，效率低（时间复杂度 O(2^n)）。
int fib_recursive(int n) {
    if (n == 0) return 0;  // 基情况：F(0) = 0
    if (n == 1) return 1;  // 基情况：F(1) = 1
    return fib_recursive(n-1) + fib_recursive(n-2);  // 递归公式
}

// 斐波那契数列循环实现
// 原理：从 F(0) 和 F(1) 开始，逐步计算后续项，避免递归的重复计算。
// 优点：效率高（时间复杂度 O(n)）；缺点：代码稍复杂。
int fib_iterative(int n) {
    if (n == 0) return 0;  // 特殊情况：F(0) = 0
    if (n == 1) return 1;  // 特殊情况：F(1) = 1
    int a = 0, b = 1, c;  // a=F(0), b=F(1)
    for (int i = 2; i <= n; i++) {  // 从 i=2 开始循环到 n
        c = a + b;  // 计算下一项
        a = b;      // 更新 a 为前一项
        b = c;      // 更新 b 为当前项
    }
    return b;  // 返回 F(n)
}

// 最大公约数递归实现
// 原理：使用欧几里得算法，gcd(a, b) = gcd(b, a % b) 当 b != 0，否则 a。
// 优点：简洁高效；缺点：递归深度可能大，但对于整数通常可接受。
int gcd_recursive(int a, int b) {
    if (b == 0) return a;
    return gcd_recursive(b, a % b);
}


//主函数程序的入口
int main(int argc, char const *argv[])
{

    // int q[]={1,2,3,4,5};
    // int w[10]={0};
    // int e[20];
    // //  函数调用
    // print_arr(q, sizeof(q)/sizeof(q[0]));//w q e 实际参数
    // print_arr(w, sizeof(w)/sizeof(w[0]));
    // print_arr(e, sizeof(e)/sizeof(e[0]));
    //-------------------------------------------------------------------------质数判断 打印质数
    // for (int i = 0,c=0,b=0; c < 100; i++)
    // {
    //     if(su(i))
    //     {
    //         printf("%d\t", i);c++;
    //     }
    // }
    // int n=8;
    // printf("%d的阶乘是%d\n", n, fact(n));
    
    // 打印杨辉三角
    // int rows = 7;
    // int pascal[7][7];
    // for(int i=0; i<rows; i++){
    //     pascal[i][0] = 1;
    //     pascal[i][i] = 1;
    //     for(int j=1; j<i; j++){
    //         pascal[i][j] = pascal[i-1][j-1] + pascal[i-1][j];
    //     }
    // }
    // for(int i=0; i<rows; i++){
    //     for(int j=0; j<=i; j++){
    //         printf("%d   ", pascal[i][j]);
    //     }
    //     printf("\n");
    // }

    printf("最大公约数是%d\n", gcd_recursive(9,108));
    return 0;
}

