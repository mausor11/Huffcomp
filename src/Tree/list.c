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


// lista nie może być NULL
int fileToList(lista_t list, FILE *in, int ile) {
	int count;
	char c;
	lista_t tmp = list;
	for(count = 0; count < ile; count++) {
		if( (fread(&c, sizeof(char), 1, in) ) != 1)
			return count;
		tmp->c = c;
		tmp->next = createList();
		tmp = tmp->next;
	}
	return count;
}






int getTreeLength(FILE *in, short ile) {
	int dlugosc = 1;
	int whatBit;
	int fs = ile;
	char grbg, cntr = 8;
	fseek(in, 5, SEEK_SET);
	fread(&grbg, sizeof(char), 1, in);
	while(fs != 0) {
		if(!cntr) {
			fread(&grbg, sizeof(char), 1, in);
			dlugosc++;
			cntr+=8;
		}
		if(bit(grbg, 7)){
			grbg <<= cntr;
			cntr--;
			fread(&grbg, sizeof(char), 1, in);
			dlugosc++;
			grbg <<= 8 - cntr;
			fs--;
		}
		else {
			grbg <<= 1;
			cntr--;
		}
	}
	return dlugosc;
}