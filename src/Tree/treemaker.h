#ifndef TREEMAKER_H_INCLUDED
#define TREEMAKER_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>

#include "tree.h"
#include "list.h"
#include "bitbajt.h"

typedef struct {
    d_t tree;
    int priority;
} table_t;


d_t extend(d_t tree, char what, char where, int);

d_t add(d_t tree, char znak);

d_t charCounter (FILE *input, d_t tree, bool Verbose);

d_t makeHTree(d_t);

void codeTree(d_t, lista_t *, char *, char *);

void readTree(d_t , lista_t *, short *, union eitbit *, char *);

#endif