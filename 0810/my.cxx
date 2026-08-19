#include <iostream>
// include<string.h>
#include <cstring>
using namespace std;
namespace my
{
    int n{3};
    int cur_n = 0;
    int *p = new int[3]{0};
    void arr()
    {
        int new_n = 2 * n;

        int *p2 = new int[new_n]{0};
        for (int i = 0; i < n; i++)
        {

            p2[i] = p[i];
        }

        delete[] p;   // 释放旧空间
        p = p2;       // 指向p2
        p2 = nullptr; // 解除p2的指向
        n=new_n;
    }
    void show(int x)
    {
        for (int i = 0; i < x; i++)
        {

            cout << p[i] << endl;
        }
    }
    void close()
    {
        delete[] my::p;
        my::p = nullptr;
    }
    void add(int add)
    {
        if (cur_n >= n)
        {
            arr();
        }
        p[cur_n] = add;
        cur_n++;
    }
}
int main()
{
    my::add(100);
    my::add(120);
    my::add(130);
    my::add(140);
    my::add(1430);
    my::add(1430);
    my::show(my::cur_n);
    my::close();
    return 0;
}
