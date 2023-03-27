#include <stdio.h>
#include "bitbajt.h"

int main(int argc, char **argv) {
	char temp;
	FILE *in = fopen(argv[1], "rb");
	while(fread(&temp, sizeof(char), 1, in)){
		printBits(temp, 8);
		printf("\n");
	}
	fclose(in);
}