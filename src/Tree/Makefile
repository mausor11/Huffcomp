libtree: tree.o bitbajt.o
	ar rvs libtree.a $^

main: main.o tree.o bitbajt.o
	$(CC) -o main main.o tree.o bitbajt.o

tree: tree.c tree.h
	$(CC) -c tree.c tree.h



main.o: main.c
	$(CC) -c main.c


test2: test2.c tree.o
	$(CC) -o test2 test2.c tree.o


test: test.c bitbajt.o
	$(CC) -o test test.c bitbajt.o

bitbajt.o: bitbajt.c bitbajt.h
	$(CC) -c bitbajt.c

.PHONY: clean

clean: main.o tree.o
	rm main.o tree.o main test.o test
