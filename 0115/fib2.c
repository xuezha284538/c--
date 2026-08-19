#include <stdio.h>
int fib(int n)
{
    int a[n];
    a[0] = 1;
    a[1] = 1;

    for (int i = 2; i < n; i++)
    {

        a[i] = a[i - 2] + a[i - 1];
    }
    return a[n - 1];
}
int fib2(int n)
{
    if (n == 1 || n == 2)
    {
        return 1;
    }
    else
    {
        return fib2(n - 1) + fib2(n - 2);
    }
}
int main(int argc, char const *argv[])
{
    printf("斐波那契数列第4个数:%d",fib(4));
    return 0;
}