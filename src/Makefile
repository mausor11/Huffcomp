test: main.o Tree/libtree.a SzyfrXOR/libxor.a Flag/libflag.a  
	$(CC) -o main main.o Tree/tree.o Flag/flag.o -L SzyfrXOR -l xor -L Tree -l tree 

main.o: main.c
	$(CC) -c main.c
clean:
	rm *.o main
