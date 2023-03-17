#include <stdio.h>

// pozdro dla zloga

int what(char resource, int which){//getbit
	int mask = 1 << which;
	return (resource & mask) ? 1 : 0;
}



int main() {
	char c = 0b00101011;
	(what(c, 0)) && (what(c, 1)) ? printf("11\n") : printf("nei\n");
	(what(c, 2)) && (what(c, 1)) ? printf("11\n") : printf("nei\n");
	printf("%c\n", c);
	char d = 0b00000100;
	c += d;
	printf("%c\n", c);
	return 0;
}