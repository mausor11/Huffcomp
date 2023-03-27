#include <stdio.h>
#include <stdlib.h>

int main() {
	char cc;
	char *c = &cc;
	*c = 'x';
	printf("%c\n", *c);
	c = NULL;
	c = &cc;
	*c = 'y';
	printf("%c\n", *c);
	return 0;
}