#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "tree.h"

int main() {
	d_t tree = createTree(), tree2;
	void *temp;
	int mini, maxi, i = 0;
	char c = 'e';
	table_t tab[4];
	tree->znak = 'a';
	tree->counter = 10;
	tree = extend(tree, 'b', 'p', 1);
	tree->right_node->counter = 100;
	tree->right_node = extend(tree->right_node, 'c', 'n', 1);
	tree->right_node->left_node->counter = 1;
	tree->right_node = extend(tree->right_node, 'd', 'p', 1);
	tree->right_node->right_node->counter = 1000;
	commonest(tree, &maxi);
	/*
	do {
		mini = maxi;
		rarest(tree, &c, &mini);
		mark(tree, c);		// <- useless
		printf("znak %c, %d wystąpień\n", c, mini);
		writeTree(tree);
		printf("\n\n");
		tab[i].priority = mini;
		i++;
	} while(mini != maxi);
	for(i = 0; i < 4; i++)
		printf("%d\n", tab[i].priority );
	*/
	printf("aa\n");
	rarestt(tree, &tree2, &mini);
	printf("aa %p\n", tree2);
	printf("%d\n", tree2->znak );
	freeTree(tree);
	return 0;
}