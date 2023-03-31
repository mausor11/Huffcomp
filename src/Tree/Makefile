tree: tree.c tree.h
	$(CC) -c tree.c tree.h

treemaker: treemaker.c treemaker.h tree.h bitbajt.h
	$(CC) -c $^

krokiet: krokiet.c krokiet.h tree.h bitbajt.h
	$(CC) -c $^

bitbajt: bitbajt.c bitbajt.h
	$(CC) -c $^


.PHONY: clean

clean:
	rm *.o main test test2 libtree.a *.gch
