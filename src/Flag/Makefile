libflag: flag.o
	ar rvs libflag.a $^
all: main.o flag.o
	$(CC) -o test main.o flag.o
main.o: main.c
	$(CC) -c main.c
flag.o: flag.c
	$(CC) -c flag.c
clean:
	rm *.o output test libflag.a
