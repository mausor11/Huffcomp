#include <stdio.h>
#include <stdlib.h>

#include "bitbajt.h"

// credit to  zlog for the idea (code written myself)

int bit(char resource, int which){	//which od 0 do 7
	int mask = 1 << which;
	return (resource & mask) ? 1 : 0;
}

char *bajt(FILE *input, int many) {
	char *ret = malloc(many * sizeof(*ret) );
	fread(ret, sizeof(char), many, input);
	return ret;
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