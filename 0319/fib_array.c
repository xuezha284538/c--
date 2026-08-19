#include <stdio.h>
#include <stdlib.h>

// 使用数组实现斐波那契数列
long long fib_array(int n) {
    if (n <= 0) return 0;
    if (n == 1 || n == 2) return 1;
    
    long long fib[n + 1];
    fib[1] = 1;
    fib[2] = 1;
    
    for (int i = 3; i <= n; i++) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }
    
    return fib[n];
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <n>\n", argv[0]);
        return 1;
    }
    
    int n = atoi(argv[1]);
    long long result = fib_array(n);
    printf("Fibonacci(%d) = %lld\n", n, result);
    
    return 0;
}
