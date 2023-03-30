test: main.o szyfr.o
	$(CC) -o test main.o szyfr.o

szyfr.o: szyfr.c szyfr.h
	$(CC) -c szyfr.c

main.o: main.c
	$(CC) -c main.c


.PHONY: clean

clean: main.o szyfr.o
	rm main.o szyfr.o test password lib*.a
