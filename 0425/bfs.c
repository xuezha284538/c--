#include <stdio.h>

int q[100];
int tail = 0, head = 0;

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

void bfs(int p[][6], int b[])
{
    while (tail > head) {
        int i = q[head++];
        for (int j = 0; j < 6; j++) {
            if (p[i][j] == 1 && b[j] == 0) {
                show(j);
                b[j] = 1;
                q[tail++] = j;
            }
        }
    }
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

void show2(int i, int b[], int a[][6])
{
    q[tail++] = i;
    b[i] = 1;
    show(i);
    bfs(a, b);
    printf("^");
}

int main()
{
    int a[6][6] = {0};
    init(a);
    int b[6] = {0};
    show2(1, b, a);

    return 0;
}
