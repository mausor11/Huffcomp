#include <stdio.h>

typedef struct{
	int a;
	int b;
} mazda;


void foo(mazda t[]) {
	t[0].a = 123;
	t[0].b = 312;
}

int main() {
	mazda t[5];
	foo(t);
	printf("%d %d\n", t[0].a, t[0].b);
	return 0;
}