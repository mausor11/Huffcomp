#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "treemaker.h"
#include "bitbajt.h"


// używane do zbierania i liczenia wystąpień znaków
d_t add(d_t tree, unsigned char znak) {
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
    short many = 0;
    counter(tree, &many);
    table_t tab[many];
    int i, maxi = 0;
    d_t root = NULL;
    commonest(tree, &maxi);		// znajdujemy znak, który występuje najczęściej
    for(i = 0; i < many; i++) {
        tab[many-1-i].priority = maxi;
        rarestt(tree, &(tab[many-1-i].tree), &(tab[many-1-i].priority));
        tab[many-1-i].tree->counter = -1;
    }
    // usuwamy połączenia między drzewem liczącym wystąpnienia
    for(i = 0; i < many; i++) {
        tab[i].tree->right_node = NULL;
        tab[i].tree->left_node = NULL;
    }

    // łączymy od dołu (najrzadszych) węzły w jedno drzewo
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
        // przesuwamy w odpowiednie miejsce w tabeli
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


void codeTree(d_t tree, FILE *output, unsigned char *temp, char *cntr) {
	if(*cntr == 8) {
		fwrite(temp, sizeof(char), 1, output);
		*temp = 0;
		*(cntr)-=8;
	}
	(*temp) <<= 1;
	(*cntr)++;
	if(tree->counter == 0) { 		// węzeł
		(*temp) += 0;
	}
	else {		// liść - trzeba użyć unii
		(*temp) += 1;
		union eitbit saas;
		saas.A = *temp;
		saas.B = tree->znak;
		saas.ab <<= (8 - *cntr);
		fwrite(&(saas.A), sizeof(char), 1, output);
		saas.ab <<= *cntr;
		*temp = saas.A;

	}
	if(tree->left_node != NULL)
		codeTree(tree->left_node, output, temp, cntr);
	if(tree->right_node != NULL)
		codeTree(tree->right_node, output, temp, cntr);
}




// odczyt zakodowanego drzewa
d_t readTree(FILE *in, short *liscie, union eitbit *temp, char *cntr) {
	if(*liscie) {			// są liście do wczytania
		d_t tree = createTree();
		int currentBit;
		if(*cntr == 0) {
			fread( &(temp->B), sizeof(char), 1 ,in);
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
			tree->znak = temp->A;
			tree->counter = -1;
			(*liscie)--;
			temp->ab <<= *cntr;
			fread( &(temp->B), sizeof(char), 1 ,in);
			temp->ab <<= (8 - *cntr);
		}
		return tree;
	}
}