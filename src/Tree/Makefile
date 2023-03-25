libtree: tree.o treemaker.o bitbajt.o list.o krokiet.o
	ar rvs libtree.a $^

main: main.o tree.o bitbajt.o
	$(CC) -o main main.o tree.o bitbajt.o

tree: tree.c tree.h
	$(CC) -c tree.c tree.h

list: list.c list.h
	$(CC) -c list.c list.h

treemaker: treemaker.c treemaker.h tree.h list.h
	$(CC) -c treemaker.c treemaker.h tree.h list.h

krokiet: krokiet.c krokiet.h tree.h
	$(CC) -c $^



main.o: main.c
	$(CC) -c main.c

test3: test3.c tree.o treemaker.o list.o bitbajt.o krokiet.o
	$(CC) -o test3 $^

test2: test2.c tree.o treemaker.o list.o bitbajt.o
	$(CC) -o test2 test2.c tree.o treemaker.o list.o bitbajt.o


test: test.c tree.o bitbajt.o list.o
	$(CC) -o test test.c tree.o bitbajt.o list.o

bitbajt.o: bitbajt.c bitbajt.h
	$(CC) -c bitbajt.c bitbajt.h

.PHONY: clean

clean:
	rm *.o main test test2 libtree.a *.gch
