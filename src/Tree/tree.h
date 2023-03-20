#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>

#include "treemaker.h"

void writeTree(d_t tree, int i);

int readTree(d_t, char *, int);

void rarest(d_t, char *, int *);

void rarestt(d_t, d_t*, int *);

void commonest(d_t, int *);

void mark(d_t, char);

void counter(d_t, int*);

#endif