#include <stdio.h>
//#include <stdlib.h>

struct lit {
	char t[6];
};

int main(int argc, char **argv) {
	FILE *in = fopen("dane", "rb");
	FILE *out = fopen("wydane", "wb");

	char c;

	struct lit abc;

	if(in == NULL || out == NULL){
		fprintf(stderr, "dumb cuck\n");
		return 1;
	}

	while(fread(&c, sizeof(char), 1, in) == 1) {	// funkcja wczytywania binarnych plikówj
		fprintf(stdout, "%d -> '%c'\n", c, c);
		fwrite(&c, sizeof(char), 1, out);
	}
	fclose(in);
	in = fopen("dane", "rb");

	while(fread(&abc, sizeof(struct lit), 1, in) == 1) {
		fwrite(&abc, sizeof(struct lit), 1, out);
	}

	/* notka do tego:

	Tutaj jest struktura mająca 6 znaków.
	Jeżeli po przejściu programu zrobi się
		$ cat wydane

	pierwsza kopia abcefg będzie ze znakiem nowej linii, drugia bez.
	Jest tak, bo znak zakończenia linii (10 w ASCII, 00001010)
	jest    s i ó d m y m    ciągiem 8-bitowym w pliku.

	*/

	fclose(in);
	fclose(out);
	return 0;
}
