#include <stdio.h>
#include <stdlib.h>

#include "bitbajt.h"

// testowanie zależności bitowych,
// funkcji bit i bajt, oraz odczytu 
// drzewa z pliku

union expanded{
	short a;
	struct{char A; char B;};
};

int main() {
	union expanded abc;
	int i;
	char tmp;
	char c = 0b00101011;
	char d = 0b00000100;
	char e = 0b10000100;
	char *tab;
	FILE *in = fopen("abcd", "rb");
	tab = bajt(in, 4);
	(bit(c, 0)) && (bit(c, 1)) ? printf("11\n") : printf("nei\n");
	(bit(c, 2)) && (bit(c, 1)) ? printf("11\n") : printf("nei\n");
	printf("%c\n", c);
	c += d;
	printf("%c\n", c);
	for(i = 0; i < 4; i++)
		printf("%c\n", *(tab+i) );
	fclose(in);

	in = fopen("abcd", "rb");
	fread(&(abc.A), sizeof(char), 1, in);
	printf("%c\n", abc.A);
	printf("%d\n", 8%8);
	fclose(in);
	i = 10*bit(e, 7) + bit(e, 6);
	printf("%d\n", i);
	
	return 0;
}