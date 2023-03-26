#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "tree.h"
#include "treemaker.h"

void foo(lista_t lista){
	if(lista != NULL){
		printf("znak: %c\n", lista->c);
		lista = lista->next;
		foo(lista);
	}
}


void oof(d_t tree, int asd){
	if(asd == 0)
		printf("asadasdasdasda %c\n", tree->znak);
	tree->left_node = createTree();
	tree->left_node->znak = 'p';
	tree->left_node->counter = -1;
	if(asd == 1) {
		oof(tree->left_node, 0);
		printf("ddfddfdffddf %c\n", tree->left_node->znak);
	}
}


int main() {
	lista_t aa = malloc(sizeof(*aa) );
	lista_t abc = aa;
	aa->c = 'x';
	aa = addToList(aa, 'y');
	aa = addToList(aa, 'z');
	aa = addToList(aa, 'a');
	aa = addToList(aa, 'b');
	aa = addToList(aa, 'c');
	printf("aa->c : %c\n", aa->c);
	foo(aa);
	printf("aa->c : %c\n", aa->c);
	freeList(aa);
	printf("\n\n=== TREE TIME ===\n\n\n");
	d_t tree = createTree();
//	tree->left_node = createTree();
//	tree->left_node->znak = 'a';
	tree->right_node = createTree();
	tree->right_node->left_node = createTree();
	tree->right_node->left_node->znak = 'c';
	tree->right_node->right_node = createTree();
	tree->right_node->right_node->znak = 'e';
	oof(tree, 1);
	writeTree(tree, 0);
	printf("%c\n", tree->left_node->znak);
//	printf("%c\n", tree->znak);
	freeTree(tree);
	return 0;
}