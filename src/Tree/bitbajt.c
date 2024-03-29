#include <stdio.h>
#include <stdlib.h>

#include "bitbajt.h"



int bit(unsigned char resource, int which){	//which od 0 do 7
	int mask = 1 << which;
	return (resource & mask) ? 1 : 0;
}

int bit2(unsigned short resource, int which){	//which od 0 do 15
    int mask = 1 << which;
    return (resource & mask) ? 1 : 0;
}

int bajt(unsigned char *ret, FILE *input, int many) {
	return fread(ret, sizeof(char), many, input);
}

int bajt2(unsigned short *ret, FILE *input, int many) {
	int how = many;
	unsigned char *aaa = malloc(3 * sizeof( *aaa) );
	while(how && (fread(aaa, sizeof(char), 3, input) == 3 ) ) {
		*(ret-how+many) = 0;
		*(ret-how+many+1) = 0;
		*(ret - how + many) += *aaa;
		*(ret - how + many) <<= 8;
		*(ret - how + many) += *(aaa+1);
		*(ret - how + many) >>= 4;
		*(ret - how + many) &= 0b0000111111111111;		// wszelki wypadek

		*(ret - how + many + 1) += *(aaa+1);
		*(ret - how + many + 1) <<= 8;
		*(ret - how + many + 1) += *(aaa+2);
		*(ret - how + many + 1) &= 0b0000111111111111;		// wszelki wypadek

		how -= 2;
	}
	free(aaa);
    return many - how;
}




int bajt3(unsigned short *ret, FILE *input, int many) {
    return fread(ret, sizeof(short), many, input);
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
