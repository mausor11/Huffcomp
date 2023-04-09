#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>

typedef struct e {
    int counter;
	unsigned char znak;
    struct e *right_node;
    struct e *left_node;

} *d_t;

typedef struct w {
    int counter;
    unsigned short znak;
    struct w *right_node;
    struct w *left_node;

} *d_t16;


d_t createTree();

d_t16 createTree16();

void freeTree(d_t tree);
void freeTree16(d_t16 tree);

void writeTree(FILE *, d_t tree, int i);
void writeTree16(FILE *, d_t16 tree, int i);

void rarestt(d_t, d_t*, int *);
void rarestt16(d_t16, d_t16*, int *);

void commonest(d_t, int *);
void commonest16(d_t16, int *);

void counter(d_t, short*);
void counter16(d_t16, short*);
void counter16int(d_t16, int*);

#endif