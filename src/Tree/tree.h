#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

typedef struct e {
    char znak;
    int counter;
    struct e *right_node;
    struct e *left_node;

} *d_t;

union eitbit{
	short shrt;
	struct {char A; char B;};
};

d_t createTree();
void freeTree(d_t tree);
d_t extend(d_t tree, char what, char where);
d_t add(d_t tree, char znak);
void writeTree(d_t tree);
d_t charCounter (FILE *input, d_t tree, bool Verbose);
//int readTree(d_t, char *, int);


#endif