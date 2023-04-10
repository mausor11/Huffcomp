#include <stdio.h>
#include "bitbajt.h"

int main() {
	unsigned short a = 0b1111000000001111;
	unsigned char b;
	b = (a >>= 4);
	printBits(b, 8);
	printf("\n");
	printBits(a, 16);
	printf("\n");
	return 0;
}