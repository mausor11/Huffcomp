#ifndef _KROKIET_H_
#define _KROKIET_H_

#include "tree.h"
#include "bitbajt.h"

/*

struktura o nazwie krokiet_t jest tablicą kodu dla
każdego znaku w kodzie ASCII. W main'ie deklarowana
jest tablica wielkości 256 struktur zaiwerających:
	- 258 elementową tablicę 'bitów' (int)
		(pierwszy i ostatni mogą teoretycznie
		 być różne od 0 i 1)
	- int oznaczający, czy kod danego znaku w tablicy
	  jest skończony, czy należy dalej dopisywać

*/
/* uwagi do struktury:
 	- kod[0] będzie zawsze równe 2
 	- od kod[1] będzie faktyczny, zero-jedynkowy kod,
 	  dopóki nie pojawi się liczba inna niż 0 lub 1.
 	  Taka liczba jest jednocześnie zakończeniem
	  oraz długością ciągu kodu.
	- reszta tablicy to śmieci
*/

typedef struct {
	char kod[258];
	int done;
} krokiet_t;


typedef struct {
    char kod[4098];
    char done;
} krokiet_t12;


typedef struct {
    char kod[65538];
    char done;
	unsigned short znak;
} krokiet_t16;


void fillKrokiet(d_t, krokiet_t [], int, int);
void fillKrokiet12(d_t16, krokiet_t12 [], int, int);
void fillKrokiet16(d_t16, krokiet_t16 *, int, int, unsigned short, unsigned short *);

void printKrokiet(FILE *, krokiet_t []);
void printKrokiet12(FILE *, krokiet_t12 []);
void printKrokiet16(FILE *, krokiet_t16 *, short);

void prepareKrokiet(krokiet_t []);
void prepareKrokiet12(krokiet_t12 []);
void prepareKrokiet16(krokiet_t16 *, unsigned short);

void emptyKrokiet(krokiet_t []);

void codeFile(krokiet_t [], FILE *, FILE *, unsigned char *, char *);
void codeFile12(krokiet_t12 [], FILE *, FILE *, unsigned short *, char *);
void codeFile16(krokiet_t16 [], FILE *, FILE *, unsigned short *, char *);

d_t decodeFile(d_t, FILE *, FILE *, union eitbit *, char *);
d_t16 decodeFile12(d_t16, FILE *, FILE *, union sixtbit *, char *, char *, unsigned char *);
d_t16 decodeFile16(d_t16, FILE *, FILE *, union sixtbit *, char *);

d_t decode(d_t, union eitbit *, char *);
d_t16 decode12(d_t16, union sixtbit *, char *);
d_t16 decode16(d_t16, union sixtbit *, char *);

void freeObiad(krokiet_t16 *);

#endif