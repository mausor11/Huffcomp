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
    short many = 0; //byl int
    counter(tree, &many);
    table_t tab[many];
    int i, maxi = 0;
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
        //printf("tab: %d\n", tab[j-1].priority);
        tab[j].priority = 0;
        j--;
        while(j > 0 && ( tab[j].priority >= tab[j-1].priority ) ) {
            tree = tab[j].tree;
            temp = tab[j].priority;
            tab[j].tree = tab[j-1].tree;
            tab[j].priority = tab[j-1].priority;
            tab[j-1].tree = tree;
            tab[j-1].priority = temp;
            j--;
        }
    }

    tree = tab[0].tree;
    tab[0].tree = NULL;

    return tree;
}


void codeTree(d_t tree, lista_t *output, char *temp, char *cntr) {
	//printf("codeTree: entered\n");
	if(*cntr == 8) {
	// potrzebny wskaźnik na początek w programie wywołującym
		*output = addToList(*output, *temp);
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

// to *powinno* stworzyć drzewo <- i tak robi :D
// cntr od 0 do 8 - liczba ważnych bitów w temp.B
//		(tak, żeby w temp.A zawsze był pełny char)
d_t readTree(lista_t in, short *liscie, union eitbit *temp, char *cntr) {
	if(*liscie) {			// są liście do wczytania
		d_t tree = createTree();
		int currentBit;
		if(*cntr == 0) {
			if((in) == NULL){return tree;}			// skończyły się dane (liście != 0 - sprawdzaj)
			temp->B = (in)->c;
			(in)  = (in)->next;
			(*cntr) += 8;
		}
		currentBit = bit(temp->A, 7);
		temp->ab <<= 1;
		(*cntr)--;
		if(currentBit == 0) {
			tree->left_node = readTree(in, liscie, temp, cntr);
			tree->right_node = readTree(in, liscie, temp, cntr);
		}

		else {
			printBits(temp->A, 8);
			tree->znak = temp->A;
			tree->counter = -1;
			(*liscie)--;
			temp->ab <<= *cntr;
			if((in) == NULL){return tree;}
			temp->B = (in)->c;
			(in) = (in)->next;
			temp->ab <<= (8 - *cntr);
		}
		return tree;
	}
}

/*
// cntr to, znów, licznik, ale teraz całego ab - należy dodać 8 w mainie, jeżeli 
char decodeFile(d_t tree, lista_t in, union eitbit *temp, char *cntr) {
	d_t current;
	int b;
	while(in != NULL) {
		current = tree;
		b = bit(temp->A, 7);
	}
}

*/