#include <stdio.h>
#include <stdlib.h>

typedef struct e {
	char znak;
	struct e *wyz;
	struct e *niz;

} *d_t;


d_t createTree() {
	d_t neww = malloc(sizeof *neww);
	neww->wyz = NULL;
	neww->niz = NULL;
	return neww;
}

void freeTree(d_t tree){
	if(tree->niz != NULL) {
		freeTree(tree->niz);
	}
	if(tree->wyz != NULL) {
		freeTree(tree->wyz);
	}
	free(tree);
}


d_t extend(d_t tree, char what, char where) {
	d_t bab = createTree();
	bab->znak = what;
	switch(where) {
		case 'r':
			bab->wyz = tree;
			break;
		case 'l':
			bab->niz = tree;
			break;
		case 'p':
			if(tree->wyz == NULL) {
				tree->wyz = bab;
				bab = tree;
			}
			break;
		default:
			if(tree->niz == NULL) {
				tree->niz = bab;
				bab = tree;
				fprintf(stdout, "%c, %c\n", bab->znak, tree->znak);
			}
			break;

	}
	return bab;
}



int main(int argc, char **argv) {
//	FILE *in = fopen(argv[1], "rb");
	d_t tab = createTree();
	void *const ptr = tab;
	tab->znak = 'a';
	d_t ntab = extend(tab, 'b', 'r');
	tab = extend(ntab, 'c', 'n');
//	tab = ptr;
	if(ntab->wyz != NULL)
		printf("dziaa: %c -> %c\n", ntab->znak, ntab->wyz->znak);
	if(tab->niz != NULL)
		printf("dziaa drugie: %c -> %c -> %c\n", tab->znak, tab->wyz->znak, tab->niz->znak);// tab->niz->wyz->znak);

	/*
	co tu się wydarzylo:
	powstalo drzewo, najwyzszy wskaznik to ntab:
		ntab->znak = b
		ntab->niz = null
		ntab->wyz->znak = tab->znak = a
		ntab->wyz->niz->znak = ntab->niz->znak = c
		reszta null
	^ nieaktualne as of 17.03.23
	*/

	freeTree(ntab);
	return 0;
}
