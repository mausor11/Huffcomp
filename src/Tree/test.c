#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int a;
	int b;
} kint;

int main(){
	unsigned short x;
	unsigned char y = 12;
	FILE *in = fopen("b", "wb+");
	fwrite(&y, sizeof(char), 1, in);
	fwrite(&y, sizeof(char), 1, in);
	fseek(in, 0, SEEK_SET);
	printf("%ld\n", fread(&x, sizeof(short), 1, in) );
	printf("%d\n", x);
	printf("%ld\n", fread(&x, sizeof(short), 1, in) );
	printf("%d\n", x);

	kint *abc = malloc(2 * sizeof(kint));
	(*abc).a = 432;
	(*abc).b = 434;
	(abc+1)->a = 653;
	(*(abc+1)).b = 123;

	printf("%d %d %d %d\n", abc->a, abc->b, abc[1].a, abc[1].b);

	free(abc);
	fclose(in);
	return 0;
}