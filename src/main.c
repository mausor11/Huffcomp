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
	char flagCmprs = 'c', flagCrypt = 'n', flagVerb = 'n';
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
				flagCmprs = 'c';
				if(Verbose == true)
					printf("==DEBUG== getopt: Chosen force compression\n");
				break;

			case 'x':
				flagCmprs = 'd';
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
	if(flagCmprs == 'c') {


/* deklaracje zmiennych */
		krokiet_t obiad[256];
		short ile = 0;
		char cntr = 0;
		unsigned char temp;
		d_t tree = NULL;


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
		codeTree(tree, output, &temp, &cntr);


/* powrót do początku pliku */
		if(fseek(input, 0, SEEK_SET) ){
			fprintf(stderr, "%s: There was an error while reading the file. Aborting.\n", argv[0]);
			return EXIT_FAILURE;
		}


/* kompresja pliku input */
		codeFile(obiad, input, output, &temp, &cntr);
		temp <<= (8 - cntr);
		fwrite(&temp, sizeof(char), 1, output);


/* dodanie inicjałów oraz właściwych flag do pliku output */
		fseek(output, 0, SEEK_SET);
		addFlag(output, flagBit, encypt, cntr, ile, magicNumber);


/* xorowanie */
        if(encypt == true) {
            fseek(output, 6, SEEK_SET);
            XOR2(output, char_number,Verbose, password);
        }


/* dodanie sumy kontrolnej */
		magicNumber = MagicNum(output,magicNumber);
		fseek(output,2,SEEK_SET);
		fwrite(&magicNumber, sizeof(char), 1, output);


/* zwolnienie alokowanej pamięci */
		freeTree(tree);

	}




	else {

/* deklaracje zmiennych */
		union eitbit trempe;
		int dlugosc;
		short liscie;
		char cntr = 8, last, flag, btFlag = 0, Flag = 0;
		char crc = 0, temp = 0;
		d_t ntree = NULL, lastTree;
		krokiet_t obiad[256];



/* sprawdzanie flagi, sumy kontrolnej i <hasła> pliku */

        checkFlag(input, &crc, &Flag, &liscie);

        fseek(input, 0, SEEK_SET);
            if(MagicNum(input,crc) == checkmagicNumber) {
                if(Verbose) {
                    printf("==DEBUG== CHECKSUM\n");
                    printf("==DEBUG==   Everythig is fine\n");
                }
            } else {
                fprintf(stderr, "File failure\n");
                return -1;
            }
        if(Flag & 0b00100000) {
            if(flagCrypt == 'y') {
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
            } else {
                printf("Password required!\n");
            }
        }



/* odtworzenie drzewa z pliku */

		fseek(input, 6, SEEK_SET);
		last = Flag & 0b00001111;

		fread(&(trempe.A), sizeof(char), 1, input);
		fread(&(trempe.B), sizeof(char), 1, input);

		ntree = readTree(input, &liscie, &trempe, &cntr);


/* dekodowanie pozostałych bajtów pliku -
   faktycznej zawartości pliku pierwotnego */

		lastTree = decodeFile(ntree, input, output, &trempe, &cntr);



/* dekodowanie ostatniego bitu,
   który może być częściowo wykorzystany */

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


/* zwalnianie pamięci */

		freeTree(ntree);

	}


	fclose(input);
	fclose(output);
	return 0;
}
