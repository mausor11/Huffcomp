#ifndef _LIST_H_
#define _LIST_H_

typedef struct f {
	char c;
	struct f *next;
} *lista_t, slist;

lista_t expandList(lista_t, char);
void freeList(lista_t);


#endif