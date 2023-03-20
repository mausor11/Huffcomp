#include <stdlib.h>

#include "list.h"

lista_t expandList(lista_t input, char c) {
	if(input != NULL) {
		lista_t tmp = input;
		tmp->next = malloc(sizeof *(tmp->next) );
		tmp->next->c = c;
		tmp->next->next = NULL;
		tmp = tmp->next;
		return tmp;
	}
	else {
		lista_t tmp = malloc(sizeof *tmp);
		tmp->next = NULL;
		tmp->c = c;
		return tmp;
	}
}

void freeList(lista_t input) {
	if(input->next != NULL)
		freeList(input->next);
	free(input);
}