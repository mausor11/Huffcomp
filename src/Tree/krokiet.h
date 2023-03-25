#ifndef _KROKIET_H_
#define _KROKIET_H_

#include "tree.h"


/* uwagi do struktury:
 	- kod[0] będzie zawsze równe 2
 	- od kod[1] będzie faktyczny, zero-jedynkowy kod
 	  dopóki nie pojawi się liczba inna niż 0 lub 1.
 	  Taka liczba jest jednocześnie zakończeniem
	  oraz długością kodu
	- reszta tablicy to śmieci
*/
typedef struct {
	int kod[258];
	int done;
} krokiet_t;

void fillKrokiet(d_t, krokiet_t [], int, int);

void printKrokiet(krokiet_t []);

void emptyKrokiet(krokiet_t []);

void codeFile(krokiet_t [], FILE *, FILE *);

#endif