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
    neww->znak = 0; //dodane
    return neww;
}

d_t16 createTree16() {
    d_t16 neww = malloc(sizeof *neww);
    neww->right_node = NULL;
    neww->left_node = NULL;
    neww->counter = 0;
    neww->znak = 0; //dodane
    return neww;
}

void freeTree(d_t tree) {
    if(tree->left_node != NULL)
        freeTree(tree->left_node);
    if(tree->right_node != NULL)
        freeTree(tree->right_node);
    free(tree);
}


// potrzeba?
void freeTree16(d_t16 tree) {
    if(tree->left_node != NULL)
        freeTree16(tree->left_node);
    if(tree->right_node != NULL)
        freeTree16(tree->right_node);
    free(tree);
}

void writeTree(d_t tree, int i) {
	if(tree != NULL) {
		i++;
		printf("====\tlevel %d | ascii num:%c; ascii char:%d; count:%d\n", i, tree->znak, tree->znak, tree->counter);
		writeTree( tree->left_node, i);
		writeTree( tree->right_node, i);
	}
}


void writeTree16(d_t16 tree, int i) {
	if(tree != NULL) {
		i++;
		printf("====\tlevel %d | number: %d; count:%d\n", i, tree->znak, tree->counter);
		writeTree16( tree->left_node, i);
		writeTree16( tree->right_node, i);
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

void rarestt16(d_t16 tree, d_t16 *temp, int *mini) {
    if(tree->right_node != NULL)
        rarestt16(tree->right_node, temp, mini);
    if(tree->left_node != NULL)
        rarestt16(tree->left_node, temp, mini);
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

void commonest16(d_t16 tree, int *maxi) {
    if(tree->right_node != NULL)
        commonest16(tree->right_node, maxi);
    if(tree->left_node != NULL)
        commonest16(tree->left_node, maxi);
    if( tree->counter > *maxi )
        *maxi = tree->counter;
}


void counter(d_t tree, short *cntr) {
	if(tree->right_node != NULL)
		counter(tree->right_node, cntr);
	if(tree->left_node != NULL)
		counter(tree->left_node, cntr);
	if(tree->counter != 0)
		(*cntr)++;
}

void counter16(d_t16 tree, short *cntr) {
    if(tree->right_node != NULL)
        counter16(tree->right_node, cntr);
    if(tree->left_node != NULL)
        counter16(tree->left_node, cntr);
    if(tree->counter != 0)
        (*cntr)++;
}