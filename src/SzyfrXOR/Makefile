libxor: szyfr.o
	ar rvs libxor.a $^
test: main.o szyfr.o
	$(CC) -o test main.o szyfr.o	
szyfr.o: szyfr.c szyfr.h
	cc -c szyfr.c
main.o: main.c
	cc -c main.c

.PHONY: clean
	
clean: main.o szyfr.o
	rm main.o szyfr.o test password
