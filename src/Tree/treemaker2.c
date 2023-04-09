#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "tree.h"
#include "treemaker2.h"
#include "treemaker.h"
#include "bitbajt.h"




void codeTree12(d_t16 tree, FILE *output, unsigned short *temp, char *cntr) {
	if(*cntr == 16) {
		fwrite(temp, sizeof(short), 1, output);
		*temp = 0;
		(*cntr)-=16;
	}
	(*temp) <<= 1;
	(*cntr)++;
	if(tree->counter == 0) { 		// węzeł
		(*temp) += 0;
	}
	else {		// liść - trzeba użyć unii
		(*temp) += 1;
		union sixtbit saas;
		saas.C = *temp;
		saas.D = tree->znak;
		saas.D <<= 4;			// pierwsze cztery bity znaku drzewa nieistotne

		if((*cntr) > 3) {			// mamy co najmniej short danych
			saas.cd <<= (16 - *cntr);
			fwrite(&(saas.C), sizeof(short), 1, output);
			(*cntr) -= 4;
			saas.cd <<= (*cntr);

			// przy odczycie trzeba będzie przesuwać o 4 do tyłu
			// i nie zapomnieć tej przesuniętej czwórki
		}
		else {
			saas.cd <<= 12;
			(*cntr)+=12;
		}

//		saas.cd <<= *cntr;
		*temp = saas.C;

	}
	if(tree->left_node != NULL)
		codeTree12(tree->left_node, output, temp, cntr);
	if(tree->right_node != NULL)
		codeTree12(tree->right_node, output, temp, cntr);
}





// odczyt zakodowanego drzewa
d_t16 readTree12(FILE *in, short *liscie, union sixtbit *temp, char *cntr) {
	if(*liscie) {			// są liście do wczytania
		d_t16 tree = createTree16();
		int currentBit;
		if(!(*cntr) ) {
			fread( &(temp->D), sizeof(short), 1 ,in);
			(*cntr) += 16;
		}
		currentBit = bit2(temp->C, 15);
		temp->cd <<= 1;
		(*cntr)--;
		if(currentBit == 0) {
			tree->left_node = readTree12(in, liscie, temp, cntr);
			tree->right_node = readTree12(in, liscie, temp, cntr);
		}

		else {
			tree->znak = temp->C;
			tree->znak >>= 4;
			tree->znak = tree->znak & 0b0000111111111111;	// na wszelki
			tree->counter = -1;
			(*liscie)--;
			if((*cntr) < 13) {
				temp->cd <<= *cntr;
				fread( &(temp->D), sizeof(short), 1 ,in);
				temp->cd <<= (12 - *cntr);
			}


			else {			// pozostaje co najmniej 1 istotny bit
							// w temp->D, nie możemy go stracić
				temp->cd <<= 12;
				(*cntr) -= 12;
			}
		}
		return tree;
	}
}


