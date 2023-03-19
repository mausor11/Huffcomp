#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "tree.h"

d_t createTree() {
    d_t neww = malloc(sizeof *neww);
    neww->right_node = NULL;
    neww->left_node = NULL;
    neww->counter = 0;
    return neww;
}

void freeTree(d_t tree) {
    if(tree->left_node != NULL) {
        freeTree(tree->left_node);
    }
    if(tree->right_node != NULL) {
        freeTree(tree->right_node);
    }
    free(tree);
}


d_t extend(d_t tree, char what, char where) {
    d_t bab = createTree();
    bab->znak = what;
    switch(where) {
        case 'r':
            bab->right_node = tree;
            break;
        case 'l':
            bab->left_node = tree;
            break;
        case 'p':
            if(tree->right_node == NULL) {
                tree->right_node = bab;
                bab = tree;
            }
            break;
        default:
            if(tree->left_node == NULL) {
                tree->left_node = bab;
                bab = tree;
                fprintf(stdout, "%c, %c\n", bab->znak, tree->znak);
            }
            break;

    }
    return bab;
}

d_t add(d_t tree, char znak) {

    if(tree == NULL) {
        d_t n_tree = createTree();
        n_tree->znak = znak;
        n_tree->counter = 1;
        return n_tree;
    }

    if(znak < tree->znak) {
        tree->left_node = add(tree->left_node, znak);
    } else if (znak > tree->znak) {
        tree->right_node = add(tree->right_node, znak);
    } else {
        tree->counter++;
    }

    return tree;
}

d_t charCounter (FILE *input, d_t tree, bool Verbose) {
    char x;
    while((x = fgetc(input)) != EOF) {
        tree = add(tree, x);
    }
    return tree;
}


void readTree(d_t tree) {

    if(tree != NULL) {

        readTree( tree->left_node);
        readTree( tree->right_node);
        printf("%c ascii:%d -> %d\n", tree->znak, tree->znak, tree->counter);

    }

}
