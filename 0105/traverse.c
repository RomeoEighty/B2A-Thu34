#include <stdio.h>
#include <stdlib.h>



typedef struct node
{
    int value;
    struct node *left;
    struct node *right;
} Node;

void traverse(const Node *n)
{
    if (n == NULL)
        return;

    printf("value = %d\n", n->value);
    if (n->value == 4)
        if (n->left != NULL)
            exit(1);

    if (n->left != NULL)
        traverse(n->left);
    if (n->right != NULL)
        traverse(n->right);
    return;
}

int main()
{
    Node n4  = {  4, NULL, NULL };
    Node n5  = {  5, NULL, NULL };
    Node n3  = {  3, &n4, &n5 };
    Node n6  = {  6, NULL, NULL };
    Node n2  = {  2, &n3, &n6 };
    Node n9  = {  9, NULL, NULL };
    Node n8  = {  8, &n9, NULL };
    Node n10 = { 10, NULL, NULL };
    Node n7  = {  7, &n8, &n10 };
    Node n1  = {  1, &n2, &n7 };

    traverse(&n1);
}
