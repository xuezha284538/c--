#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
/*
 * 实验四：哈夫曼编码/译码（C 语言实现）
 *
 * 编译：
 *   gcc -std=c11 -O2 -Wall huffman.c -o huffman
 *
 * 运行（交互式）：
 *   ./huffman
 *   输入信源符号个数 n
 *   依次输入 n 行：<符号> <概率/权值>
 *   程序输出哈夫曼码表，然后可进行编码与译码演示
 *
 * 说明：
 * - “符号”建议用不含空格的字符串（如 u1、A、B、C 等）
 * - 概率可以是任意正数（不必严格和为 1），程序按权值构建哈夫曼树
 */

typedef struct {
    double w;
    int parent;
    int lchild;
    int rchild;
    char *sym; /* 仅叶子结点有符号 */
} Node;

static void *xmalloc(size_t n) {
    void *p = malloc(n);
    if (!p) {
        fprintf(stderr, "内存分配失败\n");
        exit(1);
    }
    return p;
}

static char *xstrdup(const char *s) {
    size_t n = strlen(s) + 1;
    char *p = (char *)xmalloc(n);
    memcpy(p, s, n);
    return p;
}

/* 在 [0, m) 中选出 parent==-1 的两个最小权值结点下标 s1,s2 */
static void select_two_min(const Node *t, int m, int *s1, int *s2) {
    int i;
    int a = -1, b = -1;
    for (i = 0; i < m; i++) {
        if (t[i].parent != -1) continue;
        if (a == -1 || t[i].w < t[a].w) {
            b = a;
            a = i;
        } else if (b == -1 || t[i].w < t[b].w) {
            b = i;
        }
    }
    *s1 = a;
    *s2 = b;
}

static Node *build_huffman(const char **symbols, const double *weights, int n, int *out_total) {
    int i;
    int total = (n <= 0) ? 0 : (2 * n - 1);
    Node *t = (Node *)xmalloc((size_t)total * sizeof(Node));

    for (i = 0; i < total; i++) {
        t[i].w = 0.0;
        t[i].parent = -1;
        t[i].lchild = -1;
        t[i].rchild = -1;
        t[i].sym = NULL;
    }

    for (i = 0; i < n; i++) {
        t[i].w = weights[i];
        t[i].sym = xstrdup(symbols[i]);
    }

    if (n == 1) {
        if (out_total) *out_total = total;
        return t;
    }

    for (i = n; i < total; i++) {
        int s1, s2;
        select_two_min(t, i, &s1, &s2);
        if (s1 == -1 || s2 == -1) {
            fprintf(stderr, "构建哈夫曼树失败：权值输入可能有误\n");
            exit(1);
        }
        t[s1].parent = i;
        t[s2].parent = i;
        t[i].lchild = s1;
        t[i].rchild = s2;
        t[i].w = t[s1].w + t[s2].w;
    }

    if (out_total) *out_total = total;
    return t;
}

static char **gen_codes(const Node *t, int n) {
    int i;
    char **codes = (char **)xmalloc((size_t)n * sizeof(char *));
    for (i = 0; i < n; i++) codes[i] = NULL;

    if (n == 1) {
        codes[0] = xstrdup("0");
        return codes;
    }

    for (i = 0; i < n; i++) {
        char tmp[1024];
        int k = 0;
        int c = i;
        int p = t[c].parent;
        while (p != -1) {
            if (t[p].lchild == c) tmp[k++] = '0';
            else tmp[k++] = '1';
            c = p;
            p = t[c].parent;
            if (k >= (int)sizeof(tmp) - 1) {
                fprintf(stderr, "码长过大（n 过大）\n");
                exit(1);
            }
        }
        tmp[k] = '\0';
        /* reverse */
        {
            char *code = (char *)xmalloc((size_t)k + 1);
            int j;
            for (j = 0; j < k; j++) code[j] = tmp[k - 1 - j];
            code[k] = '\0';
            codes[i] = code;
        }
    }
    return codes;
}

static int find_symbol(const Node *t, int n, const char *sym) {
    int i;
    for (i = 0; i < n; i++) {
        if (t[i].sym && strcmp(t[i].sym, sym) == 0) return i;
    }
    return -1;
}

static char *encode_message(const Node *t, char **codes, int n, const char *line) {
    size_t cap = 1024, len = 0;
    char *out = (char *)xmalloc(cap);
    out[0] = '\0';

    char *buf = xstrdup(line);
    char *save = NULL;
    char *tok = strtok_r(buf, " \t\r\n", &save);
    while (tok) {
        int idx = find_symbol(t, n, tok);
        if (idx < 0) {
            fprintf(stderr, "未知符号：%s（请检查输入）\n", tok);
            free(buf);
            free(out);
            return NULL;
        }
        const char *c = codes[idx];
        size_t need = strlen(c);
        if (len + need + 1 > cap) {
            while (len + need + 1 > cap) cap *= 2;
            out = (char *)realloc(out, cap);
            if (!out) {
                fprintf(stderr, "内存分配失败\n");
                exit(1);
            }
        }
        memcpy(out + len, c, need);
        len += need;
        out[len] = '\0';

        tok = strtok_r(NULL, " \t\r\n", &save);
    }
    free(buf);
    return out;
}

static char *decode_bits(const Node *t, int n, const char *bits) {
    int root = (n == 1) ? 0 : (2 * n - 2);
    int cur = root;

    size_t cap = 1024, len = 0;
    char *out = (char *)xmalloc(cap);
    out[0] = '\0';

    if (n == 1) {
        /* 任意 bit(0/1) 都可解为唯一符号，这里按每个有效 bit 输出一次 */
        const char *p = bits;
        while (*p) {
            if (*p == '0' || *p == '1') {
                size_t need = strlen(t[0].sym) + 1;
                if (len + need + 1 > cap) {
                    while (len + need + 1 > cap) cap *= 2;
                    out = (char *)realloc(out, cap);
                    if (!out) exit(1);
                }
                memcpy(out + len, t[0].sym, strlen(t[0].sym));
                len += strlen(t[0].sym);
                out[len++] = ' ';
                out[len] = '\0';
            }
            p++;
        }
        return out;
    }

    for (const char *p = bits; *p; p++) {
        if (*p != '0' && *p != '1') continue;
        if (*p == '0') cur = t[cur].lchild;
        else cur = t[cur].rchild;

        if (cur < 0) {
            fprintf(stderr, "译码失败：比特串与码表不匹配\n");
            free(out);
            return NULL;
        }

        if (t[cur].lchild == -1 && t[cur].rchild == -1) {
            size_t s = strlen(t[cur].sym);
            if (len + s + 2 > cap) {
                while (len + s + 2 > cap) cap *= 2;
                out = (char *)realloc(out, cap);
                if (!out) exit(1);
            }
            memcpy(out + len, t[cur].sym, s);
            len += s;
            out[len++] = ' ';
            out[len] = '\0';
            cur = root;
        }
    }

    if (cur != root) {
        fprintf(stderr, "译码警告：比特串结束时未回到根（可能缺少末尾比特）\n");
    }

    return out;
}

static void free_all(Node *t, int total, char **codes, int n) {
    if (t) {
        for (int i = 0; i < total; i++) free(t[i].sym);
        free(t);
    }
    if (codes) {
        for (int i = 0; i < n; i++) free(codes[i]);
        free(codes);
    }
}

int main(void) {
    int n;
    printf("请输入信源符号个数 n：");
    if (scanf("%d", &n) != 1 || n <= 0) {
        fprintf(stderr, "n 输入无效\n");
        return 1;
    }

    char **symbols = (char **)xmalloc((size_t)n * sizeof(char *));
    double *weights = (double *)xmalloc((size_t)n * sizeof(double));

    printf("请依次输入 n 行：<符号> <概率/权值>\n");
    for (int i = 0; i < n; i++) {
        char buf[256];
        double w;
        if (scanf("%255s %lf", buf, &w) != 2 || w <= 0) {
            fprintf(stderr, "第 %d 行输入无效（权值需为正数）\n", i + 1);
            for (int j = 0; j < i; j++) free(symbols[j]);
            free(symbols);
            free(weights);
            return 1;
        }
        symbols[i] = xstrdup(buf);
        weights[i] = w;
    }

    int total = 0;
    Node *tree = build_huffman((const char **)symbols, weights, n, &total);
    char **codes = gen_codes(tree, n);

    printf("\n--- 哈夫曼码表 ---\n");
    for (int i = 0; i < n; i++) {
        printf("%-10s  w=%g  code=%s\n", tree[i].sym, tree[i].w, codes[i]);
    }

    /* 方便后续提示：将“符号->编码”再以紧凑形式打印一次 */
    printf("\n码表(紧凑)：");
    for (int i = 0; i < n; i++) {
        printf("%s=%s%s", tree[i].sym, codes[i], (i == n - 1) ? "" : "  ");
    }
    printf("\n");

    /* 清理 leaves 输入的 symbols（tree 中已复制一份） */
    for (int i = 0; i < n; i++) free(symbols[i]);
    free(symbols);
    free(weights);

    /* 吃掉 scanf 后的换行 */
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}

    printf("\n请输入要编码的符号序列（空格分隔，直接回车跳过）\n");
    printf("可用符号及对应编码：");
    for (int i = 0; i < n; i++) {
        printf("%s=%s%s", tree[i].sym, codes[i], (i == n - 1) ? "" : "  ");
    }
    printf("\n> ");
    char *line = NULL;
    size_t cap = 0;
    ssize_t r = getline(&line, &cap, stdin);
    if (r > 1) {
        char *bits = encode_message(tree, codes, n, line);
        if (bits) {
            printf("编码结果(bit)：\n%s\n", bits);
            free(bits);
        }
    }

    printf("\n请输入要译码的比特串（仅含 0/1，允许夹杂空格，直接回车结束）\n");
    printf("对应码表：");
    for (int i = 0; i < n; i++) {
        printf("%s=%s%s", tree[i].sym, codes[i], (i == n - 1) ? "" : "  ");
    }
    printf("\n> ");
    free(line);
    line = NULL;
    cap = 0;
    r = getline(&line, &cap, stdin);
    if (r > 1) {
        char *msg = decode_bits(tree, n, line);
        if (msg) {
            printf("译码结果(符号序列)：\n%s\n", msg);
            free(msg);
        }
    }
    free(line);

    free_all(tree, total, codes, n);
    return 0;
}
