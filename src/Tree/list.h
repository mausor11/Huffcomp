#ifndef _LIST_H_
#define _LIST_H_

#include <stdio.h>

typedef struct f {
	char c;
	struct f *next;
} *lista_t, slist;

lista_t expandList(lista_t, char);
lista_t addToList(lista_t list, char c);
lista_t createList();
void list_size(lista_t lista);
void freeList(lista_t);
void listToFile(lista_t, FILE *in);
void printList(lista_t);

#endif