#include <stdio.h>
#include <stdlib.h>

#include "bitbajt.h"

int main() {
	FILE *in = fopen("abcd", "rb");
	char *abc = malloc(100 * sizeof(*abc));
	int ile = bajt(abc, in, 2);
	ile = bajt(abc, in, 2);
	printf("ile:%d\nznak:%c\n", ile, *(abc+1));
	free(abc);
	fclose(in);
	return 0;
}