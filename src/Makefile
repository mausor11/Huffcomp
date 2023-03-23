test: main.o Tree/libtree.a SzyfrXOR/libxor.a Flag/libflag.a  
	$(CC) -o main main.o Tree/tree.o -L SzyfrXOR -l xor -L Tree -l tree -L Flag -l flag

main.o: main.c
	$(CC) -c main.c
clean:
	rm *.o main
