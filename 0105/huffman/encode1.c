#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include "encode.h"

#define NSYMBOLS 256+1

static int symbol_count[NSYMBOLS];

typedef struct node
{
    int symbol;
    int count;
    struct node *left;
    struct node *right;
} Node;

static void count_symbols(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "error: cannot open %s\n", filename);
        exit(1);
    }

    int i;
    for (i = 0; i < NSYMBOLS; i++) {
        symbol_count[i] = 0;
    }

    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        symbol_count[ch]++;
    }

    symbol_count[NSYMBOLS-1]++; // End of File

    fclose(fp);
}

static Node *pop_min(int *n, Node *nodep[])
{
    // Find the node with the smallest count
    int i;
    int j = 0;
    for (i = 0; i < *n; i++) {
        if (nodep[i]->count < nodep[j]->count) {
            j = i;
        }
    }

    Node *node_min = nodep[j];

    // Remove the node pointer from nodep[]
    for (i = j; i < (*n) - 1; i++) {
        nodep[i] = nodep[i + 1];
    }
//  printf("hoge%dhoge\n", *n);
    (*n)--;

    return node_min;
}

static Node *build_tree()
{
    int n = 0;
    Node *nodep[NSYMBOLS];

    for (int i = 0; i < NSYMBOLS; i++) {
        if (symbol_count[i] == 0)
            continue;
        nodep[n] = malloc(sizeof(Node));
        nodep[n]->symbol = i;
        nodep[n]->count  = symbol_count[i];
        nodep[n]->left   = NULL;
        nodep[n]->right  = NULL;
        n++;
    }

//  printf("n%d\n", n);
//  for (int i = 0; i < n; i++) {
//      printf("[%3d] %3d %c %d\n", i, nodep[i]->symbol, nodep[i]->symbol, nodep[i]->count);
//  }

    while (n >= 2) {
//      printf("======n%d=====\n", n);
//  for (int i = 0; i < n; i++) {
//      printf("[%3d] %3d %c %d\n", i, nodep[i]->symbol, nodep[i]->symbol, nodep[i]->count);
//  }
        Node *node1 = pop_min(&n, nodep);
        Node *node2 = pop_min(&n, nodep);

        // Create a new node
        nodep[n] = malloc(sizeof(Node));
        nodep[n]->count  = node1->count + node2->count;
//      printf("%d %d .%d---\n", node1->count, node2->count, nodep[n]->count);
        nodep[n]->symbol = -1;
        nodep[n]->left   = node1;
        nodep[n]->right  = node2;
        n++;
    }
//  int i = 0;
//  printf("[%3d] %3d %c %d\n", i, nodep[i]->symbol, nodep[i]->symbol, nodep[i]->count);

    return nodep[0];
}

/*
 * print code
 */
static void pcode(unsigned num, int length)
{
    printf("code:");
    for (int i = length - 1; i >= 0; i--) {
        const int j = num / (2 << i);
        printf("%d", j);
        num -= j * (2 << i);
    }
    puts("");
}

// Perform depth-first traversal of the tree
static void traverse_tree(const int depth, const Node *np, unsigned code)
{
    assert(depth < NSYMBOLS);

    if (np->left == NULL) {
        printf("depth:%d count:%d symbol:%c\n", depth, np->count, np->symbol);
        pcode(code, depth);
        return;
    }



    code <<= 1;
    traverse_tree(depth + 1, np->left, code);
    traverse_tree(depth + 1, np->right, ++code);
}

int encode(const char *filename)
{
    count_symbols(filename);
    Node *root = build_tree();
    traverse_tree(0, root, 0);

    return 1;
}
