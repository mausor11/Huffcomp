#ifndef _TREEMAKER2_H_
#define _TREEMAKER2_H_

#include <stdio.h>
#include <stdbool.h>

#include "tree.h"
#include "treemaker.h"
#include "bitbajt.h"

typedef struct {
    d_t16 tree;
    int priority;
} table12_t;

// d_t16 add16(d_t16 tree, unsigned short znak);

// d_t charCounter (FILE *input, d_t tree, bool Verbose);

// d_t16 makeHTree16(d_t16);

void codeTree12(d_t16, FILE *, unsigned short *, char *);

d_t readTree(FILE *, short *, union eitbit *, char *);

d_t16 readTree12(FILE *, short *, union sixtbit *, char *);

#endif