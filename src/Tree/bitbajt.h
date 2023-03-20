#ifndef _BITBAJT_H_
#define _BITBATJ_H_

#include <stdio.h>

union eitbit {
	short ab;
	struct {char A; char B;};
};

int bit(char, int );
char *bajt(FILE *, int );
void secwet();

#endif