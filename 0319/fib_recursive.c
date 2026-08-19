#include <stdio.h>
#include <stdlib.h>

// 使用递归实现斐波那契数列
long long fib_recursive(int n) {
    if (n <= 0) return 0;
    if (n == 1 || n == 2) return 1;
    
    return fib_recursive(n - 1) + fib_recursive(n - 2);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <n>\n", argv[0]);
        return 1;
    }
    
    int n = atoi(argv[1]);
    long long result = fib_recursive(n);
    printf("Fibonacci(%d) = %lld\n", n, result);
    
    return 0;
}
