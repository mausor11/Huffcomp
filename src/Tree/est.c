#include <stdio.h>

#include "bitbajt.h"

int main() {
	FILE *in = fopen("abcd", "rb");
	char *abc = bajt(in, 1000);
	printf("%c\n", *(abc+5));
	return 0;
}