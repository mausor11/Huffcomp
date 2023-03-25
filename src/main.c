#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>


#include "SzyfrXOR/szyfr.h"
#include "Tree/tree.h"
#include "Tree/treemaker.h"
#include "Tree/bitbajt.h"
#include "Tree/list.h"
#include "Tree/krokiet.h"
#include "Flag/flag.h"


char *usage =
	"Usage: %s [options] input_file output_file\n"
	"	List of options:\n"
	"		* -z - force compressoion;\n"
	"		* -x - force decompression;\n"
	"			Warning: if both -z and -x are chosen,\n"
	"				 last one will be executed.\n"
	"		* -o - compression type:\n"
	"			+ 1 - 8 bit compression;\n"
	"			+ 2 - 12 bit compression;\n"
	"			+ 3 - 16 bit compression;\n"
	"		* -c - encrypt output file;\n"
    "			+ password;\n"
	"		* -v - print additional info into stdout;\n"
	"		* -h - print help.\n";

bool Verbose = false;

void setVerbose () {
    Verbose = true;
}

int main(int argc, char **argv) {

	srand(time(NULL));
	int char_number = 8;
	int flagBit = 0;
	int opt;
	int i;
	char *password;
	char flagComp = 'n', flagCrypt = 'n', flagVerb = 'n';
	bool encypt = false;
	FILE *input;
	FILE *output;

	if(argc < 2){
		fprintf(stderr, "%s: Not enough arguments!\n\n%s\n", argv[0], usage);
		return 1;
	}
	if(argc < 3){
		fprintf(stderr, "%s: Not enough arguments to open file!\n\n%s\n", argv[0], usage);
		return 1;
	}

	input = fopen(argv[argc-2], "rb");
	output = fopen(argv[argc-1], "wb+");

	//sprawdzanie czy jest flaga -v i włączanie verbose
	for(i=1;i<argc;i++) {
		if(strcmp(argv[i], "-v") == 0) {
			setVerbose();
			break;
		}
	}
	if(Verbose) {
		printf("==DEBUG== Huffman coding\n");
		printf("==DEBUG== Copyright (C), by Bartosz Dańko and Jan Machowski\n");
		printf("==DEBUG== Command: ");
		for(i=0;i<argc;i++)
			printf("%s ", argv[i]);
		printf("\n");
	}
	secwet();
	if(input == NULL) {
		fprintf(stderr, "%s: File %s is not open!\n",argv[0], argv[argc-2]);
		return -1;
	} else if(output == NULL) {
		fprintf(stderr, "%s: FILE %s is not open!\n",argv[0], argv[argc-1]);
		return -1;
	}

	if(Verbose) {
		printf("==DEBUG==\n");
		printf("==DEBUG== INPUT-OUTPUT FILES\n");
		printf("==DEBUG==   %s -> %s\n",argv[argc-2], argv[argc-1]);
	}

	while ( (opt = getopt (argc, argv, "o:c:vhxz") ) != -1 ) {
		switch(opt) {
			case 'o':
				flagBit = atoi (optarg);
				if(Verbose == true) {
					printf("==DEBUG==\n");
					printf("==DEBUG== COMPRESSION LEVEL\n");
					printf("==DEBUG==   getopt: Chosen option -o %d\n", flagBit);
				}
				switch(flagBit) {
					case 2:
						char_number = 16;
						break;
					case 3:
						char_number = 32;
						break;
					default:	// 1 albo inne
						char_number = 8;
						break;
				}

				if(Verbose){ printf("==DEBUG== Bit number: %d\n", char_number); };
				break;
			case 'c':
				password = optarg;
                encypt = true;
				flagCrypt = 'y';
				if(Verbose) {
					printf("==DEBUG==\n");
					printf("==DEBUG== getopt: Chosen option -c. Changes to %c\n", flagCrypt);
					printf("==DEBUG==\n");
				}
                //zaraz na dol
				//XOR(input, output, char_number, Verbose, password);
				break;
			case 'v':
				break;
			case 'z':
				flagComp = 'c';
				if(Verbose == true)
					printf("==DEBUG== getopt: Chosen force compression\n");
				break;
			case 'x':
				flagComp = 'd';
				if(Verbose == true)
					printf("==DEBUG== getopt: Chosen force decompression\n");
				break;
			case 'h':
				fprintf(stdout, usage, argv[0]);
				return 0;
			default:
				fprintf(stderr, usage, argv[0]);
				exit(EXIT_FAILURE);
		}
	}
    // WORK IN PROGRESS


	lista_t zakod = NULL;
	d_t tree = NULL;
	krokiet_t obiad[256];

	char cntr = 0;
	char temp;


	while((fread(&temp,sizeof(char),1,input)) == 1) {
		tree = add(tree, temp);
    }
    //writeTree(tree, 0);
    //d_t tmp = tree;
    tree = makeHTree(tree);
    //writeTree(tree, 0);
    temp = 0;
    char ile = 0;
    counter(tree, &ile);
	writeTree(tree,0);

	prepareKrokiet(obiad);
	fillKrokiet(tree, obiad, 0, -2);
	printKrokiet(obiad);


    codeTree(tree, &zakod, &temp, &cntr);
//	temp <<= (8 - cntr);
//	drzewo = addToList(drzewo, temp);
//	lista_t tm = drzewo;
//	list_size(tm);
/*	while(tm != NULL) {
        //printf("%c\n", tm->c);
        printBits(tm->c,8);
        printf("|");
        tm = tm->next;

    }
*/
//    printBits(temp, cntr);
//    printf("\nIstotne bity ostatniego znaku - %d\n", cntr);


	fclose(input);
	input = fopen(argv[argc-2], "rb");

//	cntr = 0;

//	printf("==Zakodowany plik input:\n");
	codeFile(obiad, input, &zakod, &temp, &cntr);
//	printf("\nOstatni znak, cyfry znaczące - %d: ", cntr);

	temp <<= (8-cntr);
	zakod = addToList(zakod, temp);

//	printBits(temp, 8);
//	printf("\n");

	addFlag(output,flagBit,encypt,cntr, &ile);

	listToFile(zakod, output);
	lista_t tm = zakod;
	list_size(tm);

    while(tm != NULL) {
        //printf("%c\n", tm->c);
        printBits(tm->c,8);
        printf("|");
        tm = tm->next;
	}

	printf("bits used in last: %d\n", cntr);

	checkFlag(output);

	printf("aa\n");

    fclose(input);
    fclose(output);
    freeList(zakod);
    freeTree(tree);
	printf("\n\n=============================================\n\n");
	input = fopen(argv[argc-1], "rb");
	while( (fread(&cntr, sizeof(char), 1, input) ) == 1 ) {
		printBits(cntr, 8);
		printf(" - %c\n", cntr);
	}
	fclose(input);
	return 0;
}
