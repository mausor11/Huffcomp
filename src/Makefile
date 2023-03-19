test: main.o SzyfrXOR/libxor.a Tree/bitbajt.o Tree/tree.o
	$(CC) -o main main.o Tree/bitbajt.o Tree/tree.o -L SzyfrXOR -l xor
main.o: main.c
	$(CC) -c main.c
clean:
	rm *.o main
