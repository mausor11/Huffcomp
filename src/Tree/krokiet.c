#include <stdio.h>
#include <stdlib.h>

#include "krokiet.h"
#include "tree.h"
#include "bitbajt.h"

// czym jest krokiet? -> krokiet.h



void fillKrokiet(d_t tree, krokiet_t obiad[], int poziom, int what) {
	// jesteśmy w węźle - wszystkie kody znaków w nim dostają zero
	if(tree->counter == 0) {
		for(int i = 0; i < 256; i++)
			if(obiad[i].done != 1)
				obiad[i].kod[poziom] = what;
	}
	// znaleźliśmy liść - zakończyć dla tego znaku
	else {
		obiad[tree->znak].kod[poziom] = what;
		obiad[tree->znak].kod[poziom+1] = -1;			// znacznik końca kodu
		obiad[tree->znak].done = 1;						// oznaczenie, że dla tego znaku jest skończony kod
	}
	if(tree->left_node != NULL)
		fillKrokiet(tree->left_node, obiad, poziom+1, 0);
	if(tree->right_node != NULL)
		fillKrokiet(tree->right_node, obiad, poziom+1, 1);
}


void fillKrokiet12(d_t16 tree, krokiet_t12 obiad[], int poziom, int what) {
	// jesteśmy w węźle - wszystkie kody znaków w nim dostają zero
	if(tree->counter == 0) {
		for(int i = 0; i < 4096; i++)
			if(obiad[i].done != 1)
				obiad[i].kod[poziom] = what;
	}
	// znaleźliśmy liść - zakończyć dla tego znaku
	else {
		obiad[tree->znak].kod[poziom] = what;
		obiad[tree->znak].kod[poziom+1] = -1;
		obiad[tree->znak].done = 1;
	}
	if(tree->left_node != NULL)
		fillKrokiet12(tree->left_node, obiad, poziom+1, 0);
	if(tree->right_node != NULL)
		fillKrokiet12(tree->right_node, obiad, poziom+1, 1);
}


void fillKrokiet16(d_t16 tree, krokiet_t16 *obiad, int poziom, int what, unsigned short ile, unsigned short *whichDone) {
    // jesteśmy w węźle - wszystkie kody znaków w nim dostają zero
    if(tree->counter == 0) {
        for(int i = 0; i < ile; i++)
            if((obiad+i)->done != 1)
                (obiad+i)->kod[poziom] = what;
    }
        // znaleźliśmy liść - zakończyć dla tego znaku
    else {
        (obiad + (*whichDone))->kod[poziom] = what;
        (obiad + (*whichDone))->kod[poziom+1] = -1;
        (obiad + (*whichDone))->done = 1;
		(obiad + (*whichDone))->znak = tree->znak;
        (*whichDone)++;
    }
    if(tree->left_node != NULL)
        fillKrokiet16(tree->left_node, obiad, poziom+1, 0, ile, whichDone);
    if(tree->right_node != NULL)
        fillKrokiet16(tree->right_node, obiad, poziom+1, 1, ile, whichDone);
}


////////////////////////////////////////////////////////////////////


void prepareKrokiet(krokiet_t obiad []) {
	for(int i = 0; i < 256; i++) {
		obiad[i].done = -1;
		for(int j = 0; j < 258; j++)
			obiad[i].kod[j] = -1;
	}
}


void prepareKrokiet12(krokiet_t12 obiad []) {
	for(int i = 0; i < 4096; i++) {
		obiad[i].done = -1;
		for(int j = 0; j < 4098; j++)
			obiad[i].kod[j] = -1;
	}
}

void prepareKrokiet16(krokiet_t16 *obiad, unsigned short ile) {
    for(int i = 0; i < ile; i++) {
        (obiad+i)->done = -1;
        for(int j = 0; j < 65538; j++)
            (obiad+i)->kod[j] = -1;
    }
}


////////////////////////////////////////////////////////////////


void printKrokiet(FILE *in, krokiet_t obiad[]) {
	for(int i = 0; i < 256; i++)
		if(obiad[i].done == 1) {
			fprintf(in, "==== %c - ", i);
			for(int j = 1; ( (obiad[i].kod[j] == 0) || (obiad[i].kod[j] == 1) ) ; j++)
				fprintf(in, "%d", obiad[i].kod[j]);
			fprintf(in, "\n");
		}
}



void printKrokiet12(FILE *in, krokiet_t12 obiad[]) {
	for(int i = 0; i < 4096; i++)
		if(obiad[i].done == 1) {
			fprintf(in, "==== short: %d - ", i);
			for(int j = 1; ( (obiad[i].kod[j] == 0) || (obiad[i].kod[j] == 1) ); j++)
				fprintf(in, "%d", obiad[i].kod[j]);
			fprintf(in, "\n");
		}
}


void printKrokiet16(FILE *in, krokiet_t16 obiad[], short ile) {
    for(int i = 0; i < ile; i++)
        if((obiad +i)->done == 1) {
        // patrz tu
            fprintf(in, "==== short: %d - ", (obiad + i)->znak);
            for(int j = 1; ( ((obiad +i)->kod[j] == 0) || ((obiad +i)->kod[j] == 1) ); j++ )
                fprintf(in, "%d", (obiad +i)->kod[j]);
            fprintf(in, "\n");
        }
}


///////////////////////////////////////////////////////////////////////


void codeFile(krokiet_t obiad[], FILE *in, FILE *out, unsigned char *temp, char *cntr) {
	unsigned char *buf = malloc(100 * sizeof (*buf) );
	int liczba = bajt(buf, in, 100);
	while(liczba != 0) {
		for(int i = 0; i < liczba; i++) {
			int j = 1;
			while( (obiad[*(buf+i)].kod[j] == 0) || (obiad[*(buf+i)].kod[j] == 1 ) ){

				// zapełnienie temp
				if(*cntr == 8) {
					fwrite(temp, sizeof(char), 1, out);
					*temp = 0;
					(*cntr) -= 8;
				}

				(*temp) <<= 1;
				(*temp) += obiad[*(buf+i)].kod[j++];
				(*cntr)++;
			}
		}

		liczba = bajt(buf, in, 100);	// kolejna dawka danych
	}
	free(buf);
}


void codeFile12(krokiet_t12 obiad[], FILE *in, FILE *out, unsigned short *temp, char *cntr) {
	unsigned short *buf = malloc(100 * sizeof (*buf) );
	int liczba = bajt2(buf, in, 100);			// bajt2 - wczytuje liczba (max100) "12bit" short do buf
	while(liczba) {
		for(int i = 0; i < liczba; i++) {
			int j = 1;
			while( (obiad[*(buf+i)].kod[j] == 0) || (obiad[*(buf+i)].kod[j] == 1) ) {

				// zapełnienie temp
				if(*cntr == 16) {
					fwrite(temp, sizeof(short), 1, out);
					*temp = 0;
					(*cntr) -= 16;
				}

				(*temp) <<= 1;
				(*temp) += obiad[*(buf+i)].kod[j++];
				(*cntr)++;
			}
		}

		liczba = bajt2(buf, in, 100);	// kolejna dawka danych

//		if(liczba > 0 &&  ( *(buf+liczba-1) != lastCheck ) ) {
//			lastCheck = *(buf+liczba-1);
//		}
		// ^ nie wykona tego, jeżeli wczytało niepełny short na koniec

	}
	free(buf);
}


void codeFile16(krokiet_t16 *obiad, FILE *in, FILE *out, unsigned short *temp, char *cntr) {
    unsigned short *buf = malloc(100 * sizeof (*buf) );
    int liczba = bajt3(buf, in, 100);
    while(liczba != 0) {
        for(int i = 0; i < liczba; i++) {
			int k = 0;
			while( *(buf+i) != (obiad + k)->znak) {		// mało prawdopodobne, że nie ma tu znalezionego znaku
				k++;
			}
            int j = 1;
			while( ((obiad+k)->kod[j] == 0) || ( (obiad+k)->kod[j] == 1) ) {

                // zapełnienie temp
                if(*cntr == 16) {
                    fwrite(temp, sizeof(short), 1, out);
                    *temp = 0;
                    (*cntr) -= 16;
                }

                (*temp) <<= 1;
                (*temp) += (obiad+k)->kod[j++];
                (*cntr)++;
            }
        }

        liczba = bajt3(buf, in, 100);	// kolejna dawka danych
    }
    free(buf);
}


//////////////////////////////////////////////////////////////////////////////////////////////


d_t decodeFile(d_t tree, FILE *in, FILE *out, union eitbit *temp, char *cntr) {
	int liczba, i;
	unsigned char recieved;
	unsigned char *buf = malloc(100 * sizeof(*buf) ), *cahr = NULL;
	d_t tremp = tree;
	while(liczba = fread(buf, sizeof(char), 100, in) ) {
		i = 0;
		while(i < liczba) {
			cahr = NULL;
			while(cahr == NULL) {
				if(!(*cntr)) {
					if(i < liczba) {
						temp->B = *(buf + i++);
						(*cntr) += 8;
					}
					else
						break;		// jeżeli to, to ostatni bajt jest w temp->A
				}
				tremp = decode(tremp, temp, cntr);
				if((tremp->counter) ) {
					recieved = tremp->znak;
					cahr = &recieved;
					fwrite(cahr, sizeof(char), 1, out);
					tremp = tree;
				}
			}
		}
	}
	if(cahr != NULL)
		tremp = tree;
	free(buf);
	return tremp;
}


// dekodowanie znaku
d_t decode(d_t tree, union eitbit *temp, char *cntr) {
	d_t trer = tree;
	while((*cntr)){
		if(!bit(temp->A, 7) ){
			temp->ab <<= 1;
			(*cntr)--;
			trer = trer->left_node;
			if(trer->counter)
				return trer;
		}
		else {
			temp->ab <<= 1;
			(*cntr)--;
			trer = trer->right_node;
			if(trer->counter)
				return trer;
		}
	}
	return trer;
}


d_t16 decodeFile12(d_t16 tree, FILE *in, FILE *out, union sixtbit *temp, char *cntr, char *whether, unsigned char *outchar) {
	int liczba, i;
	unsigned short recieved;
	unsigned short *buf = malloc(100 * sizeof(*buf) ), *cahr = NULL;
	d_t16 tremp = tree;
	while(liczba = fread(buf, sizeof(short), 100, in) ) {
		i = 0;
		while(i < liczba) {
		cahr = NULL;
            while(cahr == NULL) {
                if(!(*cntr)) {
                    if(i < liczba) {
                        temp->D = *(buf + i++);
                        (*cntr) += 16;
                    }
                    else
                        break;		// jeżeli to, to ostatni short jest w temp->C
                }
                tremp = decode16(tremp, temp, cntr);
                if((tremp->counter) ) {
                	if(!(*whether)) {		// outchar pusty
	                    recieved = tremp->znak;
    	                cahr = &recieved;
    	                (*outchar) = 0;
    	                (*outchar) = (recieved >>= 4);				// środkowe 8 bitów shorta
    	                fwrite(outchar, sizeof(char), 1, out);
						printBits((*outchar), 8);
						printf("\n");
	                    recieved = tremp->znak;
						recieved &= 0b0000000000001111;			// wszelki
						(*outchar) = recieved;
						(*whether) = 1;				// outchar ma coś w sobie;
					}
					else {
						recieved = tremp->znak;
						cahr = &recieved;
						recieved >>= 8;
						recieved &= 0b0000000000001111;			// na wszelki
						(*outchar) <<= 4;
						(*outchar) += recieved;
    	                fwrite(outchar, sizeof(char), 1, out);		// część z pierwszej dwunastki, część z drugiej
						printBits((*outchar), 8);
						printf("\n");
						recieved = tremp->znak;
						recieved &= 0b0000000011111111;			// wszelki
						(*outchar) = 0;
						(*outchar) += recieved;
    	                fwrite(outchar, sizeof(char), 1, out);
						printBits((*outchar), 8);
						printf("\n");

						(*whether) = 0;								// outchar pusty
					}
   	                tremp = tree;
                }
            }
        }
    }
    if(cahr != NULL)
        tremp = tree;
    free(buf);
    return tremp;
}


d_t16 decodeFile16(d_t16 tree, FILE *in, FILE *out, union sixtbit *temp, char *cntr) {
	int liczba, i;
	unsigned short recieved;
	unsigned short *buf = malloc(100 * sizeof(*buf) ), *cahr = NULL;
	d_t16 tremp = tree;
	while(liczba = fread(buf, sizeof(short), 100, in) ) {
		i = 0;
		while(i < liczba) {
		cahr = NULL;
            while(cahr == NULL) {
                if(!(*cntr)) {
                    if(i < liczba) {
                        temp->D = *(buf + i++);
                        (*cntr) += 16;
                    }
                    else
                        break;		// jeżeli to, to ostatni bajt jest w temp->A
                }
                tremp = decode16(tremp, temp, cntr);
                if((tremp->counter) ) {
                    recieved = tremp->znak;
                    cahr = &recieved;
                    fwrite(cahr, sizeof(short), 1, out);
                    tremp = tree;
                }
            }
        }
    }
    if(cahr != NULL)
        tremp = tree;
    free(buf);
    return tremp;
}


d_t16 decode16(d_t16 tree, union sixtbit *temp, char *cntr) {
    d_t16 trer = tree;
    while((*cntr)){
        if(!bit2(temp->C, 15) ){
            temp->cd <<= 1;
            (*cntr)--;
            trer = trer->left_node;
            if(trer->counter) {
                return trer;
            }
        }
        else {
            temp->cd <<= 1;
            (*cntr)--;
            trer = trer->right_node;
            if(trer->counter) {
                return trer;
			}
        }
    }
    return trer;
}

void freeObiad(krokiet_t16 obiad[]) {
    free(obiad);
}

