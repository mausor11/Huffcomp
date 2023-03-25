#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>

typedef struct e {
    int counter;
    char znak;
    struct e *right_node;
    struct e *left_node;

} *d_t;


d_t createTree();

void freeTree(d_t tree);

void writeTree(d_t tree, int i);

int readTree(d_t, char *, int);

// void rarest(d_t, char *, int *);

void rarestt(d_t, d_t*, int *);

void commonest(d_t, int *);

void counter(d_t, char*);


#endif