#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "tree.h"
#include "treemaker.h"
#include "bitbajt.h"


d_t createTree() {
    d_t neww = malloc(sizeof *neww);
    neww->right_node = NULL;
    neww->left_node = NULL;
    neww->counter = 0;
    return neww;
}

void freeTree(d_t tree) {
    if(tree->left_node != NULL)
        freeTree(tree->left_node);
    if(tree->right_node != NULL)
        freeTree(tree->right_node);
    free(tree);
}

void writeTree(d_t tree, int i) {
	if(tree != NULL) {
		i++;
		writeTree( tree->left_node, i);
		writeTree( tree->right_node, i);
		printf("poziom %d | %c ascii:%d -> %d\n", i, tree->znak, tree->znak, tree->counter);
	}
}

void rarest(d_t tree, char *temp, int *mini) {
	if(tree->right_node != NULL)
		rarest(tree->right_node, temp, mini);
	if(tree->left_node != NULL)
		rarest(tree->left_node, temp, mini);
	if( (tree->counter != -1 ) && ( tree->counter <= *mini) ) {
		*mini = tree->counter;
		*temp = tree->znak;
	}
}

void rarestt(d_t tree, d_t *temp, int *mini) {
	if(tree->right_node != NULL)
		rarestt(tree->right_node, temp, mini);
	if(tree->left_node != NULL)
		rarestt(tree->left_node, temp, mini);
	if( (tree->counter != -1 ) && ( tree->counter <= *mini) ) {
		*mini = tree->counter;
		*temp = tree;
	}
}


void commonest(d_t tree, int *maxi) {
	if(tree->right_node != NULL)
		commonest(tree->right_node, maxi);
	if(tree->left_node != NULL)
		commonest(tree->left_node, maxi);
	if( tree->counter > *maxi )
		*maxi = tree->counter;
}


void counter(d_t tree, char *cntr) {
	if(tree->right_node != NULL)
		counter(tree->right_node, cntr);
	if(tree->left_node != NULL)
		counter(tree->left_node, cntr);
	if(tree->counter != 0)
		(*cntr)++;
}

/*
void makeTable(d_t tree, tabt_t *tab, int *which, char path) {
	path
	if(tree->counter == 0){
		
	}
	else {

	}
	if(tree->left_node != NULL)
		makeTable(tree->left_node, tab, which);
	if(tree->right_node != NULL)
		makeTable(tree->right_node, tab, which);

}
^ not worth
*/