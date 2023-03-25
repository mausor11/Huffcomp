#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "tree.h"
#include "treemaker.h"
#include "list.h"
#include "bitbajt.h"
#include "krokiet.h"

int main() {
	lista_t lista = NULL;
	d_t tree = createTree();
	char ile = 0, cntr = 0;
	char c = 'e', temp = 0;
	table_t tab[4];

	krokiet_t obiad[256];

	// insert
	tree->znak = 'a';
	tree->counter = 10;
	tree = extend(tree, 'b', 'p', 1);
	tree->right_node->counter = 100;
	tree->right_node = extend(tree->right_node, 'c', 'n', 1);
	tree->right_node->left_node->counter = 1;
	tree->right_node = extend(tree->right_node, 'd', 'p', 1);
	tree->right_node->right_node->counter = 1000;
	counter(tree, &ile);
	printf("Drzewo ze wczytania\n\nliczba liści drzewa: %d\n\n", ile);
	writeTree(tree, 0);
	tree = makeHTree(tree);
	/*
	printf("\nDrzewo Huffmana\n\n");
	writeTree(tree, 0);
	ile = 0;
	counter(tree, &ile);
	printf("%d\n\n", ile);
	*/
	FILE *out = fopen("tutej_drzewo", "wb");
	codeTree(tree, &lista, &temp, &cntr);
	printf("\n");
	lista_t tm = lista;
	list_size(tm);
	while(tm != NULL) {
		//printf("%c\n", tm->c);
		printBits(tm->c,8);
        printf("|");
		tm = tm->next;

	}
	printBits(temp, cntr);
	printf("\n");

	emptyKrokiet(obiad);
	fillKrokiet(tree, obiad, 0, 2);

	printKrokiet(obiad);

	freeTree(tree);
	freeList(lista);
	free(tm);
	fclose(out);
	return 0;
}