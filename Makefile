test: main.o SzyfrXOR/libxor.a
	$(CC) -o main main.o -L SzyfrXOR -l xor
main.o: main.c
	$(CC) -c main.c
clean:
	rm *.o main
