test: main.o Tree/libtree.a SzyfrXOR/libxor.a 
	$(CC) -o main main.o Tree/tree.o -L SzyfrXOR -l xor -L Tree -l tree

main.o: main.c
	$(CC) -c main.c
clean:
	rm *.o main
