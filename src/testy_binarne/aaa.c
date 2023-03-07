#include <stdio.h>


int main() {
	char a = 0b01110101;
	char b = 0b00110011;

	printf("jako liczby: %d, %d\njako znaki: %c, %c\n", a, b, a, b);

	char c = a&b;

	printf("\na&b: %d, czyli %c\n", c, c);
	c = a^b;
	printf("a^b: %d, czyli %c\n", c, c);

	b = b<<1;
	printf("\nb po przesunięciu o 1 w lewo: %d, czyli %c\n", b, b);
	return 0;
}

