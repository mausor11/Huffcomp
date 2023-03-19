#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>

#include "SzyfrXOR/szyfr.h"
#include "Tree/tree.h"
#include "Tree/bitbajt.h"


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

	FILE *input = fopen(argv[argc-2], "r");
	FILE *output = fopen(argv[argc-1], "w");

	if(argc < 2){
		fprintf(stderr, "%s: Not enough arguments!\n\n%s\n", argv[0], usage);
		return 1;
	}
	if(argc < 3){
		fprintf(stderr, "%s: Not enough arguments to open file!\n\n%s\n", argv[0], usage);
		return 1;
	}

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
				flagCrypt = 'y';
				if(Verbose) {
					printf("==DEBUG==\n");
					printf("==DEBUG== getopt: Chosen option -c. Changes to %c\n", flagCrypt);
					printf("==DEBUG==\n");
				}
				XOR(input, output, char_number, Verbose, password);
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

	return 0;
}
