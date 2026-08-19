#include <iostream>
// include<string.h>
#include <cstring>
using namespace std;

int main()
{
    int n{3};
    int new_n = 0;
    int *p = new int[3]{0};
    p[0] = 100;
    p[1] = 200;
    p[2] = 600;

    if (p[n - 1] != 0)
    {
        new_n = 2 * n;
        int *p2 = new int[new_n]{0};
        for (int i = 0; i < n; i++)
        {

            p2[i] = p[i];
        }

        delete[] p;   // 释放旧空间
        p = p2;       // 指向p2
        p2 = nullptr; // 解除p2的指向
    }
    p[3] = 800;
    p[4] = 640;
    if (new_n != 0)
    {
        for (int i = 0; i < new_n; i++)
        {

            cout << p[i] << endl;
        }
    }
    else
    {

        for (int i = 0; i < n; i++)
        {

            cout << p[i] << endl;
        }
    }

    delete[] p;
    p = nullptr;
    return 0;
}

