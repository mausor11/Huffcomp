#ifndef _BITBAJT_H_
#define _BITBATJ_H_

#include <stdio.h>

#pragma pack(push,1)

union eitbit {
	short ab;
	struct {char B; char A;};
};

#pragma pack(pop)

typedef struct {
	char znak;
	char kod;
	int ile;
} tabt_t;

int bit(char, int );
char *bajt(FILE *, int );
void secwet();
void printBits( unsigned int n, int b );
#endif