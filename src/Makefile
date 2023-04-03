all: main.o Tree/libtree.a SzyfrXOR/libxor.a Flag/libflag.a
	$(CC) -o main main.o Tree/tree.o -L SzyfrXOR -l xor -L Tree -l tree -L Flag -l flag

main.o: main.c
	$(CC) -c $^

Tree/libtree.a: Tree/tree.o Tree/treemaker.o Tree/bitbajt.o Tree/krokiet.o
	ar rvs Tree/libtree.a $^

SzyfrXOR/libxor.a: SzyfrXOR/szyfr.o
	ar rvs SzyfrXOR/libxor.a $^

Flag/libflag.a: Flag/flag.o
	ar rvs Flag/libflag.a $^

clean:
	rm */*.o *.o main */lib*.a
