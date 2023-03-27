#include <stdio.h>

int main(){
	FILE *out = fopen("aaaaaaaaaa", "w");
	for(int i = 0; i < 15; i++){
		for(int j = 0; j < 100000; j++) {
			fprintf(out, "%c", 'A'+i);
		}
	}
	fclose(out);
}