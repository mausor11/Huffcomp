
#ifndef _BITBAJT_H_
#define _BITBAJT_H_

#include <stdio.h>

#pragma pack(push,1)

union eitbit {
	short ab;
	struct {unsigned char B; unsigned char A;};
};

union sixtbit {
    int cd;
    struct {unsigned short D; unsigned short C;};
};

#pragma pack(pop)

int bit(unsigned char, int );
int bit2(unsigned short, int );

int bajt(unsigned char *, FILE *, int );
int bajt2(unsigned short *, FILE *, int);
int bajt3(unsigned short *, FILE *, int);

void printBits( unsigned int n, int b );

#endif