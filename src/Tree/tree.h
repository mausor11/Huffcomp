#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

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
void writeTree(d_t tree);
d_t charCounter (FILE *input, d_t tree, bool Verbose);
//int readTree(d_t, char *, int);
void rarest(d_t, char *, int *);
void commonest(d_t, int *);
void mark(d_t, char);
void counter(d_t, int*);

#endif