#ifndef TREEMAKER_H_INCLUDED
#define TREEMAKER_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>

#include "tree.h"
#include "bitbajt.h"

typedef struct {
    d_t tree;
    int priority;
} table_t;


d_t add(d_t tree, unsigned char znak);

d_t charCounter (FILE *input, d_t tree, bool Verbose);

d_t makeHTree(d_t);

void codeTree(d_t, FILE *, unsigned char *, char *);

d_t readTree(FILE *, short *, union eitbit *, char *);

#endif