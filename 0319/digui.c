#include <stdio.h>
int fib(int n)
{
    if (n == 1 || n == 2)
    {
        return 1;
    }
    else
    {
        return fib(n - 1) + fib(n - 2);
    }
}
int fib2(int n)
{
    if (n == 0)
    {
        return 1;
        /* code */
    }

    return n * fib2(n - 1);
}

int main(int argc, char *argv[])
{
    int n;
    scanf("%d", &n);
    fib2(n);
    printf("%d",fib(n));
    return 0;
}