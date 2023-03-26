#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include "bitbajt.h"

lista_t expandList(lista_t input, char c) {
	if(input != NULL) {
		lista_t tmp = input;
		tmp->next = malloc(sizeof(slist));
		tmp->next->c = c;
		tmp->next->next = NULL;
		tmp = tmp->next;
		return tmp;
	}
	else {
		lista_t tmp = malloc(sizeof(slist));
		tmp->next = NULL;
		tmp->c = c;
		return tmp;
	}
}

lista_t createList() {
    lista_t list = malloc(sizeof(slist));
    list->c = 0;
    list->next = NULL;
    return list;
}

lista_t addToList(lista_t list, char c) {
	if(list == NULL) {
		list = createList();
		list->c = c;
		return list;
	} else {
        lista_t tmp = list;
        while(tmp->next != NULL) { tmp = tmp->next; }
		tmp->next = malloc(sizeof(slist));
		tmp->next->next = NULL;
		tmp->next->c = c;
		return list;
	}
}

void list_size(lista_t lista) {
    int counter = 0;
    if(lista == NULL) {
        printf("List size: %d\n", counter);
        return;
    } else {
        lista_t tmp = lista;
        do {
            counter++;
            tmp = tmp->next;
        } while(tmp != NULL);
        printf("List size: %d\n", counter);
        return;
    }
}

void freeList(lista_t input) {
	if(input->next != NULL)
		freeList(input->next);
	free(input);
}


void listToFile(lista_t input, FILE *output){
	while(input != NULL){
		fwrite(&(input->c), sizeof(char), 1, output);
		input = input->next;
	}
}


void printList(lista_t list) {
	lista_t tmp = list;
	while(tmp != NULL) {
		printBits(tmp->c, 8);
		printf(" | ");
		tmp = tmp->next;
	}
	printf("\n");
}