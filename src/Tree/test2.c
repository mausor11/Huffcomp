#include <stdio.h>
#include <stdlib.h>
#include "bitbajt.h"

int main() {
	unsigned short a = 0b1111000000001111;
	unsigned char b;
	b = (a >>= 4);
	printBits(b, 8);
	printf("\n");
	printBits(a, 16);
	printf("\n");

	unsigned char *aaa = malloc(2 * sizeof(char) );

	FILE *in = fopen("b", "rb+");
	FILE *out = fopen("c", "wb+");

	fread(aaa, sizeof(char), 2, in);
	printf("%c%c\n", *aaa, *(aaa+1) );
	fwrite(aaa, sizeof(char), 2, out);


	fclose(in);
	fclose(out);
	free(aaa);
	return 0;
}