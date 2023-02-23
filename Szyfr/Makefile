test: main.o szyfr.o
	cc -o test main.o szyfr.o
main.o: main.c
	cc -c main.c
szyfr.o: szyfr.c
	cc -c szyfr.c
clean: main.o szyfr.o
	rm main.o szyfr.o test
