// BST  Binary Search Tree
// 二叉搜索树
#include <stdio.h>
#include <stdlib.h>

// 节点
typedef struct node {
    int data;
    struct Node* left;   // 左孩子
    struct Node* right;  // 右孩子

} Node, Tree, node;

// 操作
// void init(Tree *t);
Node* insert(Node* root, int data);
void pre_order(Node* root);
void post_order(Node* root);
void mid_order(Node* root);
void ceng_order(Node* root);
int max(Node* root);
int min(Node* root);

int main()
{
    Tree* root = NULL;
    root = insert(root, 12);
    insert(root, 6);
    insert(root, 4);
    insert(root, 5);
    insert(root, 9);
    insert(root, 2);
    insert(root, 55);
    insert(root, 98);
    pre_order(root);
    printf("\n");
    post_order(root);
    printf("\n");
    mid_order(root);
    printf("\n");
    printf("%d", min(root));
    printf("\n");
    printf("%d", max(root));

    return 0;
}

/*void init(Tree *t)
{
    t->data=0;
    t->left=NULL;
    t->right=NULL;

}*/
node* insert(Node* root, int data)
{
    if (root == NULL) {
        root = malloc(sizeof(node));
        root->data = data;
        root->left = NULL;
        root->right = NULL;
        return root;
    }

    if (data < root->data) {
        root->left = insert(root->left, data);
    } else if (data > root->data) {
        root->right = insert(root->right, data);
    }
    return root;
}

void mid_order(Node* root)
{
    if (root != NULL) {
        mid_order(root->left);
        printf("%d,", root->data);
        mid_order(root->right);
    }
}

void pre_order(Node* root)
{
    if (root != NULL) {
        printf("%d,", root->data);
        pre_order(root->left);
        pre_order(root->right);
    }
}

void post_order(Node* root)
{
    if (root != NULL) {
        post_order(root->left);
        post_order(root->right);
        printf("%d,", root->data);
    }
}

int max(Node* root)
{
    if (root == NULL) return -1;
    while (root->right != NULL) {
        root = root->right;
    }
    return root->data;
}

int min(Node* root)
{
    if (root == NULL) return -1;
    while (root->left != NULL) {
        root = root->left;
    }
    return root->data;
}
