
#ifndef _BITBAJT_H_
#define _BITBAJT_H_

#include <stdio.h>

#pragma pack(push,1)

union eitbit {
	short ab;
	struct {unsigned char B; unsigned char A;};
};

#pragma pack(pop)

int bit(char, int );
int bajt(char *,FILE *, int );
void secwet();
void printBits( unsigned int n, int b );

#endif