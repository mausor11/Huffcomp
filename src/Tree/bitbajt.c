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
	for(int i = 0; i < many; i++) {
		fread(ret+i, sizeof(char), 1, input);
	}
	return ret;
}

