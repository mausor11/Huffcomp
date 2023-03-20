#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "tree.h"
#include "treemaker.h"
#include "bitbajt.h"

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


void mark(d_t tree, char c) {
	if(tree->znak == c) {
		tree->counter = -1;
		return;
	}
	if(tree->right_node != NULL)
		mark(tree->right_node, c);
	if(tree->left_node != NULL)
		mark(tree->left_node, c);
}

void counter(d_t tree, int *cntr) {
	if(tree->right_node != NULL)
		counter(tree->right_node, cntr);
	if(tree->left_node != NULL)
		counter(tree->left_node, cntr);
	if(tree->counter != 0)
		(*cntr)++;
}

//d_t recTree(d_t tree, int bit, ) -trzeba? niżej to samo robię xd me is fine, frfrdef

