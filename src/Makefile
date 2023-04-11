all: main.o Tree/libtree.a SzyfrXOR/libxor.a Flag/libflag.a
	$(CC) -o Huffcomp main.o Tree/tree.o -L SzyfrXOR -l xor -L Tree -l tree -L Flag -l flag
main.o: main.c
	$(CC) -c $^

Tree/libtree.a: Tree/tree.o Tree/treemaker.o Tree/bitbajt.o Tree/krokiet.o
	ar rvs Tree/libtree.a $^

SzyfrXOR/libxor.a: SzyfrXOR/szyfr.o
	ar rvs SzyfrXOR/libxor.a $^

Flag/libflag.a: Flag/flag.o
	ar rvs Flag/libflag.a $^
test: test0 test1 test2 test3 scenario
	./test
	rm PlikiTestowe/test1 PlikiTestowe/test2 PlikiTestowe/test3 PlikiTestowe/test1e PlikiTestowe/test2e PlikiTestowe/test3e PlikiTestowe/test0 PlikiTestowe/pic1d.png PlikiTestowe/pic2d.png PlikiTestowe/pic3d.png PlikiTestowe/scenario1
test0:
	./Huffcomp -o 0 PlikiTestowe/Pan_Tadeusz PlikiTestowe/test0
test1:
	./Huffcomp -o 1 PlikiTestowe/Pan_Tadeusz PlikiTestowe/out1
	./Huffcomp PlikiTestowe/out1 PlikiTestowe/test1
	./Huffcomp -o 1 PlikiTestowe/no_way.png PlikiTestowe/pic1
	./Huffcomp PlikiTestowe/pic1 PlikiTestowe/pic1d.png
	./Huffcomp -o 1 -c password PlikiTestowe/Pan_Tadeusz PlikiTestowe/out1e
	./Huffcomp -c password PlikiTestowe/out1e PlikiTestowe/test1e
	rm PlikiTestowe/out1 PlikiTestowe/out1e PlikiTestowe/pic1
test2:
	./Huffcomp -o 2 PlikiTestowe/Pan_Tadeusz PlikiTestowe/out2
	./Huffcomp PlikiTestowe/out2 PlikiTestowe/test2
	./Huffcomp -o 2 PlikiTestowe/no_way.png PlikiTestowe/pic2
	./Huffcomp PlikiTestowe/pic2 PlikiTestowe/pic2d.png
	./Huffcomp -o 2 -c password PlikiTestowe/Pan_Tadeusz PlikiTestowe/out2e
	./Huffcomp -c password PlikiTestowe/out2e PlikiTestowe/test2e
	rm PlikiTestowe/out2 PlikiTestowe/out2e
test3:
	./Huffcomp -o 3 PlikiTestowe/Pan_Tadeusz PlikiTestowe/out3
	./Huffcomp PlikiTestowe/out3 PlikiTestowe/test3
	./Huffcomp -o 3 PlikiTestowe/no_way.png PlikiTestowe/pic3
	./Huffcomp PlikiTestowe/pic3 PlikiTestowe/pic3d.png
	./Huffcomp -o 3 -c password PlikiTestowe/Pan_Tadeusz PlikiTestowe/out3e
	./Huffcomp -c password PlikiTestowe/out3e PlikiTestowe/test3e
	rm PlikiTestowe/out3 PlikiTestowe/out3e
test4:	PlikiTestowe/tescior
	./Huffcomp PlikiTestowe/tescior PlikiTestowe/tescior
scenario: PlikiTestowe/tescior 
	./Huffcomp PlikiTestowe/tescior PlikiTestowe/scenario1		
clean:
	rm */*.o *.o Huffcomp */lib*.a verboseDump
