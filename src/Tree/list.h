#ifndef _LIST_H_
#define _LIST_H_

typedef struct f {
	char c;
	struct f *next;
} *lista_t, slist;

lista_t expandList(lista_t, char);
lista_t addToList(lista_t list, char c);
void list_size(lista_t lista);
void freeList(lista_t);


#endif