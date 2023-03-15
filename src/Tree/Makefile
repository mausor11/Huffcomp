test: main.o tree.o
	$(CC) -o test main.o tree.o
tree: tree.c
	$(CC) -c tree.c
main.o: main.c
	$(CC) -c main.c
.PHONY: clean

clean: main.o tree.o
	rm main.o tree.o test
