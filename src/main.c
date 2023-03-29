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
	char flagComp = 'c', flagCrypt = 'n', flagVerb = 'n';
    unsigned char magicNumber = 69;
    unsigned char checkmagicNumber = 69;
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

	input = fopen(argv[argc-2], "rb+");
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
                setVerbose();
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

// kompresja
	if(flagComp == 'c') {


/* deklaracje zmiennych */
		krokiet_t obiad[256];
		short ile = 0;
		char cntr = 0, temp;
		d_t tree = NULL;
		lista_t zakod = NULL, beginning, pliczek = NULL;


/* liczenie wszystkich znaków w input */
		while((fread(&temp,sizeof(char),1,input)) == 1) {
			tree = add(tree, temp);
		}


/* tworzenie drzewa huffmana i tablicy alfabetowej*/
		tree = makeHTree(tree);
		temp = 0;
		counter(tree, &ile);
		addFlag(output, flagBit, encypt, cntr, ile, magicNumber);
        prepareKrokiet(obiad);
		fillKrokiet(tree, obiad, 0, -2);

/* zakodowanie i zapisanie drzewa */
		codeTree(tree, &zakod, &temp, &cntr);
		beginning = zakod;
		temp <<= (8 - cntr);
		listToFile(beginning, output);

/* powrót do początku pliku */
		if(fseek(input, 0, SEEK_SET) ){
			fprintf(stderr, "%s: There was an error while reading the file. Aborting.\n", argv[0]);
			return EXIT_FAILURE;
		}


/* kompresja pliku input */
		temp >>= (8 - cntr);
		codeFile(obiad, input, output, &temp, &cntr);
		temp <<= (8 - cntr);
		fwrite(&temp, sizeof(char), 1, output);

/* dodanie inicjałów, sumy kontrolnej, oraz właściwych flag do pliku output */
		fseek(output, 0, SEEK_SET);
		addFlag(output, flagBit, encypt, cntr, ile, magicNumber);
		magicNumber = MagicNum(output,magicNumber);
		fseek(output,2,SEEK_SET);
		fwrite(&magicNumber, sizeof(char), 1, output);
/* xorowanie */
        if(encypt == true) {
            fseek(output, 6, SEEK_SET);
            XOR2(output, char_number,Verbose, password);
        }


/* zwolnienie alokowanej pamięci */
		freeList(beginning);
		freeTree(tree);
	}




// WORK IN PROGRESS

	else {
		union eitbit trempe;
		int dlugosc;
		short liscie;
		char cntr = 8, Flag = 0, crc = 0, temp = 0, last, btFlag = 0;
		d_t ntree = NULL, lastTree;
		lista_t wagonik = createList(), lokomotywa;
		krokiet_t obiad[256];

        checkFlag(input, &crc, &Flag, &liscie);

        if(Flag & 0b00100000) {
            unsigned char tre = Flag;
            tre = tre & 0b11000000;
            tre >>= 6;
            switch(tre) {
                case 2:
                    btFlag = 12;
                    break;
                case 3:
                    btFlag = 16;
                    break;
                default:
                    btFlag = 8;
                    break;
            }
            fseek(input, 6, SEEK_SET);
            XOR2(input, btFlag,Verbose, password);
        }

        fseek(input, 0, SEEK_SET);
        /* Sprawdzenie czy plik nie uległ awarii */
        if(Verbose) {
            if(MagicNum(input,crc) == checkmagicNumber) {
                printf("==DEBUG== CHECKSUM\n");
                printf("==DEBUG==   Everythig is fine\n");
            } else {
                printf("==DEBUG== CHECKSUM\n");
                printf("==DEBUG== Something went wrong!\n");
                printf("==DEBUG==     File failure\n");
            }
        }



		fseek(input, 6, SEEK_SET);
		last = Flag & 0b00001111;

        /* sczyt do 100 elem. pliku do listy */

		dlugosc = getTreeLength(input, liscie);
		fseek(input, 6, SEEK_SET);
	    dlugosc = fileToList(wagonik, input, dlugosc + 1);
	    lokomotywa = wagonik;


		trempe.A = wagonik->c;
		trempe.B = wagonik->next->c;
		wagonik = wagonik->next->next;
		ntree = readTree(&wagonik, &liscie, &trempe, &cntr);
//		printf("\n\nNowe drzewo:\n");
//		writeTree(ntree, 0);



//		fseek(input, 6 + dlugosc - 1, SEEK_SET);		// na wszelki wypadek,
														// usunąć -1 jak będzie dobra flaga
//		fread(&temp, sizeof(char), 1, input);
//		printBits(temp, 8);
//		printf("\n");
//		dlugosc = fileToList(wagonik, input, 100);

		lastTree = decodeFile(ntree, input, output, &trempe, &cntr);

//		printBits(trempe.A, 8);
//		printf("\n");

// ta sama
		last+=cntr;
		while(last) {
			if(lastTree->counter) {
				fwrite( &(lastTree->znak), sizeof(char), 1, output);
				lastTree = ntree;
			}
			if(!bit(trempe.A, 7) ) {
				trempe.ab <<=1;
				last--;
				lastTree = lastTree->left_node;
			}
			else {
				trempe.ab <<=1;
				last--;
				lastTree = lastTree->right_node;
			}
		}
		fwrite(&(lastTree->znak), sizeof(char), 1, output);

//		fclose(input);
//		input = fopen(argv[argc-2], "rb");

//		cntr = 0;


//		printf("bits used in last: %d\n", cntr);


//		printf("\n\nbeginning: %c\n", beginning->c);




		freeTree(ntree);

//		printf("\n\n=============================================\n\n");
//		input = fopen(argv[argc-1], "rb");
//		while( (fread(&cntr, sizeof(char), 1, input) ) == 1 ) {
//			printBits(cntr, 8);
//			printf(" - %c\n", cntr);
//		}
//		fclose(input);

		freeList(lokomotywa);
	}

	fclose(input);
	fclose(output);
	return 0;
}
