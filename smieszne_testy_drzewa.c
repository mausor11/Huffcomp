#include <stdio.h>
#include <stdlib.h>

typedef struct e {
	char a;
	struct e *wyz;
	struct e *niz;

} *d_t;

int main(int argc, char **argv) {
	FILE *in = fopen(argv[1], "rb");
	char c;
	d_t tab = malloc(sizeof *tab);
	void *const ptr = tab;
	tab->wyz = NULL;
	tab->niz = NULL;
	fread(&(tab->a), sizeof(char), 1, in);
	if(in == NULL) { fprintf(stderr, "cep\n");return 1; }
	while((fread(&c, sizeof(c), 1, in)) == sizeof(c) ){
		tab = ptr;
		if(c > tab->a) {
			printf("bigga\n");
		} else {printf("smolla/saem\n");}
	}
	fclose(in);
	return 0;
}
