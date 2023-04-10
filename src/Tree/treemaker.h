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

typedef struct {
    d_t16 tree;
    int priority;
} table_t16;
// ^ priority queue


d_t add(d_t tree, unsigned char znak);
d_t16 add16(d_t16 tree, unsigned short znak);

d_t charCounter (FILE *input, d_t tree, bool Verbose);

d_t makeHTree(d_t);
d_t16 makeHTree16(d_t16);

void codeTree(d_t, FILE *, unsigned char *, char *);
void codeTree12(d_t16, FILE *, unsigned short *, char *);
void codeTree16(d_t16, FILE *, unsigned short *, char *);

d_t readTree(FILE *, short *, union eitbit *, char *);
d_t16 readTree12(FILE *, short *, union sixtbit *, char *);
d_t16 readTree16(FILE *, short *, union sixtbit *, char *);


#endif