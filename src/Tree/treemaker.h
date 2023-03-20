#ifndef TREEMAKER_H_INCLUDED
#define TREEMAKER_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>

typedef struct e {
    int counter;
    char znak;
    struct e *right_node;
    struct e *left_node;

} *d_t;

typedef struct {
    d_t tree;
    int priority;
} table_t;

union eitbit{
    int abcd;
    struct {char A; char B; char C; char D;};
};

d_t createTree();

void freeTree(d_t tree);

d_t extend(d_t tree, char what, char where, int);

d_t add(d_t tree, char znak);

d_t charCounter (FILE *input, d_t tree, bool Verbose);

d_t makeHTree(d_t);

#endif