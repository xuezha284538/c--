#include <stdio.h>
#include <stdlib.h>

// 使用for循环实现斐波那契数列
long long fib_loop(int n) {
    if (n <= 0) return 0;
    if (n == 1 || n == 2) return 1;
    
    long long a = 1, b = 1, c;
    for (int i = 3; i <= n; i++) {
        c = a + b;
        a = b;
        b = c;
    }
    
    return b;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <n>\n", argv[0]);
        return 1;
    }
    
    int n = atoi(argv[1]);
    long long result = fib_loop(n);
    printf("Fibonacci(%d) = %lld\n", n, result);
    
    return 0;
}
