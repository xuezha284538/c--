#include <stdio.h>

// v={ABCDEF}
// e={(A,B),(A,E),(A,C),(B,E),(C,F),(F,D),(E,D)}
//   A B C D E F
// A[0 1 1 0 1 0]
// B[1 0 0 0 1 0]
// C[1 0 0 0 0 1]
// D[0 0 0 0 1 1]
// E[0 1 0 1 0 0]
// F[0 0 1 1 0 0]

void init(int p[][6])
{
    p[0][1] = p[1][0] = 1;    // A-B
    p[0][4] = p[4][0] = 1;    // A-E
    p[0][2] = p[2][0] = 1;    // A-C
    p[1][4] = p[4][1] = 1;    // B-E
    p[2][5] = p[5][2] = 1;    // C-F
    p[5][3] = p[3][5] = 1;    // F-D
    p[4][3] = p[3][4] = 1;    // E-D
}

void show(int i)
{
    switch (i) {
        case 0: printf("A->"); break;
        case 1: printf("B->"); break;
        case 2: printf("C->"); break;
        case 3: printf("D->"); break;
        case 4: printf("E->"); break;
        case 5: printf("F->"); break;
        default: break;
    }
}

void di(int p[][6], int i, int j, int b[])
{
    if (b[0] == 1 && b[1] == 1 && b[2] == 1 && b[3] == 1 && b[4] == 1 && b[5] == 1) {
        return;
    }

    if (i >= 6 || j >= 6) return;

    for (int k = 0; k < 6; k++) {
        if (p[i][k] == 1 && b[k] == 0) {
            show(k);
            b[k] = 1;
            di(p, k, 0, b);
        }
    }
}

void show2(int i, int j, int b[], int a[][6])
{
    b[i] = 1;
    show(i);
    di(a, i, j, b);
    printf("^");
}

int main()
{
    int a[6][6] = {0};
    init(a);
    int b[6] = {0};
    show2(1, 0, b, a);

    return 0;
}
