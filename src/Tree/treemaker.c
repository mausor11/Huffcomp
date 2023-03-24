#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "treemaker.h"
#include "bitbajt.h"
#include "list.h"

// używane do odtworzenia (sposób 'patrz na brata')
d_t extend(d_t tree, char what, char where, int whether) {
    d_t bab = createTree();
    if(!whether)
        bab->counter = 0;
    else {
        bab->znak = what;
        bab->counter = 1;
    }
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
            }
            break;

    }
    return bab;
}

// używane do zbierania i liczenia wystąpień znaków
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

// make Huffman tree
d_t makeHTree (d_t tree){
    char many = 0; //byl int
    counter(tree, &many);
    table_t tab[many];
    int i, maxi;
    d_t root = NULL;
    commonest(tree, &maxi);
    for(i = 0; i < many; i++) {
        tab[many-1-i].priority = maxi;
        rarestt(tree, &(tab[many-1-i].tree), &(tab[many-1-i].priority));
        tab[many-1-i].tree->counter = -1;
    }
    for(i = 0; i < many; i++) {
        tab[i].tree->right_node = NULL;
        tab[i].tree->left_node = NULL;
    }
    // at this moment we've got a priority queueueueueueue as tab
    for(i = 0; tab[1].tree != NULL; i++) {
        int j = many-i-1, temp;
        tree = createTree();
        tree->counter = 0;
        tree->right_node = tab[j].tree;
        tree->left_node = tab[j-1].tree;
        tab[j].tree = NULL;
        tab[j-1].tree = tree;
        tab[j-1].priority += tab[j].priority;
        tab[j].priority = 0;
        j--;
        while(j > 0 && ( tab[j].priority >= tab[j-1].priority ) ) {
            tree = tab[j].tree;
            temp = tab[j].priority;
            tab[j].tree = tab[j-1].tree;
            tab[j].priority = tab[j-1].priority;
            tab[j-1].tree = tree;
            tab[j-1].priority = temp;
        }
    }

    tree = tab[0].tree;
    tab[0].tree = NULL;

    return tree;
}


void codeTree(d_t tree, lista_t *output, char *temp, int *cntr) {
	//printf("codeTree: entered\n");
	if(*cntr == 8) {
	// potrzebny wskaźnik na początek w programie wywołującym
		*output = expandList(*output, *temp);
		*temp = 0;
		*(cntr)-=8;
	}
	*temp <<= 1;
	(*cntr)++;
	//fprintf(stdout, "counter: %d\n\n", *cntr);
	if(tree->counter == 0) {
		*temp += 0;
	}
	else {
		*temp += 1;
		union eitbit saas;
		saas.A = *temp;
//        printBits(saas.A,6);
		saas.B = tree->znak;
//        printBits(saas.B,8);
		saas.ab <<= (8 - *cntr);
//        printBits(saas.A,8);
//		*output = expandList( *output, saas.A);
        *output = addToList( *output, saas.A);
		saas.ab <<= *cntr;
		*temp = saas.A;

	}
	if(tree->left_node != NULL)
		codeTree(tree->left_node, output, temp, cntr);
	if(tree->right_node != NULL)
		codeTree(tree->right_node, output, temp, cntr);
}

/*
void commpress(d_t tree, FILE *input, FILE *output) {
	d_t begin = tree;
	char = 0;
	int cntr = 0;
	while( (fread(&temp
}
*/

/*
 decTrec(d_t *tree, lista_t input, union eitbit *temp, int *cntr) {
	
	(*temp).A = input->c;
	(*temp).B = input->next->c;

}
*/

// W O R K    I N    P R O G R E S S
// last - ostatni przeczytany znak z tablicy( jeżeli skończone wcześniej)
// slide - przesunięcie tego znaku w bitach
/*
int readTree(d_t tree, char *tab, int many, char last, char slide) {
	union eitbit unia;
	int i = 0;		// całokształtne przesunięcie
	int twobit = 0;		// temp na dwa bity operacyjne
	int x = 0;		// cykliczne przesunięcie
	//int xx = 0;		// dodatek do przesunięcia, zmienny więc jest zmienna - slide
	//char c;			// temp do wyjęcia danych (do usunięcia) = last
	d_t *root = tree;		// dać tree o 2 nullach i znaku w sobie (aka liść)
	d_t *last = tree;
	do {
		slide = 0;
		unia.A = *(tab+i);
		unia.B = *(tab+i+1);
		unia.C = *(tab+i+2);
		unia.D = *(tab+i+3);
		unia.abcd << x;
		twobit = 10*bit(unia.A, 7) + bit(unia.A, 6);
		unia.shrt << 2;
		slide += 2;
//		last = unia.A;
		switch(twobit) {
			case 0:		// brat głęboki węzeł
				break;
			case 1:		// brat pojedynczy węzeł
				break;
			case 10:	// brat liść
				if(tree == root) {
					tree = extend(tree, unia.A, 'l', 0);
					root = tree;
					tree = extend(tree, unia.A, 'p', 1);
				}
				else {		//to drzewo ma 2 rzeczy
					tree = extend(tree, unia.A, 'p', 1);
				}
				unia.abcd << 8;
				slide += 8;
				break;
			case 11:	// brat liść + koniec
				i++;
				return i;
				break;
		}
		x = (x+slide)%8;		// always 0-7
		i+=2;
		if(x == 0)
			i++;
		last = *(tab + i);
	}while( i < many - 4);			// sprawdź
	return -1;

}
*/