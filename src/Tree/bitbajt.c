#include <stdio.h>
#include <stdlib.h>

#include "bitbajt.h"



int bit(unsigned char resource, int which){	//which od 0 do 7
	int mask = 1 << which;
	return (resource & mask) ? 1 : 0;
}

int bit2(unsigned short resource, int which){	//which od 0 do 7
    int mask = 1 << which;
    return (resource & mask) ? 1 : 0;
}

int bajt(unsigned char *ret, FILE *input, int many) {
	return fread(ret, sizeof(char), many, input);
}

int bajt2(unsigned short *ret, FILE *input, int many) {
    return fread(ret, sizeof(short), many, input);
}

void secwet() {
	FILE *in = fopen(".mysteriousFile", "w");
	fprintf(in, "Yay, you found me :3");
	fclose(in);
}

void printBits( unsigned int n, int b )
{
    const int Bits = b;
    char tmp[ Bits + 1 ];

    for( int i = 0; i < Bits; ++i )
    {
        tmp[ Bits - i - 1 ] = '0' + n % 2;
        n /= 2;
    }

    tmp[ Bits ] = 0;
    printf("%s",tmp);
}
