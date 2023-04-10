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
	"Usage: ./Huffcomp [options] input_file output_file\n"
	"	List of options:\n"
	"		* -z - force compressoion;\n"
	"		* -x - force decompression;\n"
	"			Warning: if both -z and -x are chosen,\n"
	"				 last one will be executed.\n"
	"		* -o - compression type:\n"
	"			+ 0 - no compression, copying file;\n"
	"			+ 1 - 8-bit compression;\n"
	"			+ 2 - 12-bit compression;\n"
	"			+ 3 - 16-bit compression;\n"
	"		* -c - encrypt output file;\n"
	"			+ password;\n"
	"		* -v - print additional info into stdout;\n"
	"		* -h - print help.\n";

bool Verbose = false;

bool Comp = false, Decomp = false;

	void setVerbose () {
	Verbose = true;
}

int main(int argc, char **argv) {
	srand(time(NULL));
	int charNumber = -1;
	int flagBit = 1;		// default 8-bit
	int opt;
	int i;
	unsigned char empty;
	char *password;
	char sameName = 0;
	char flagCmprs = 'n', flagCrypt = 'n', flagVerb = 'n';
	unsigned char magicNumber = 69;
	unsigned char checkmagicNumber = 69;
	bool encypt = false;
	FILE *input = NULL;
	FILE *output = NULL;
	FILE *dump = NULL;

	if(argc < 2){
		fprintf(stderr, "%s: Not enough arguments!\n\n%s\n", argv[0], usage);
		return 1;
	}
	if(argc < 3){
		fprintf(stderr, "%s:\tNot enough arguments to open file!\n\n%s\n", argv[0], usage);
		return 1;
	}

	if((strcmp(argv[argc-1], argv[argc-2]) )) {
		input = fopen(argv[argc-2], "rb+");
		output = fopen(argv[argc-1], "wb+");
	}
	else {
		fprintf(stderr, "%s\t: Cannot operate on the same file.\n", argv[0]);
		return -1;
	}

//sprawdzanie czy jest flaga -v i włączanie verbose
	for(i=1;i<argc;i++) {
		if(strcmp(argv[i], "-v") == 0) {
			setVerbose();
			dump = fopen("verboseDump", "w+");
			break;
		}
	}

	if(Verbose) {
		printf("==== Huffcomp - Huffman compressor & decompressor\n");
		printf("==== Copyright (C), by Bartosz Dańko and Jan Machowski\n");
		printf("==== Command: ");
		for(i=0;i<argc;i++)
			printf("%s ", argv[i]);
		printf("\n");
	}
	if(input == NULL) {
		fprintf(stderr, "%s: File %s is not open!\n",argv[0], argv[argc-2]);
		return -1;
	} else if(output == NULL) {
		fprintf(stderr, "%s: File %s is not open!\n",argv[0], argv[argc-1]);
		return -1;
	}

	if(Verbose) {
		printf("====\n");
		printf("==== INPUT-OUTPUT FILES\n");
		printf("====   %s -> %s\n",argv[argc-2], argv[argc-1]);
	}

// opcje
	while ( (opt = getopt (argc, argv, "o:c:vhxz") ) != -1 ) {
		switch(opt) {
			case 'o':
				flagBit = atoi (optarg);
				if(Verbose == true) {
					printf("====\n");
					printf("==== COMPRESSION LEVEL\n");
					printf("====   getopt: Chosen option -o %d\n", flagBit);
				}
				switch(flagBit) {
					case 0:
						charNumber = 0;
						break;
					case 2:
						charNumber = 12;
						break;
					case 3:
						charNumber = 16;
						break;
					default:	// 1 albo inne
						charNumber = 8;
						flagBit = 1;
						break;
				}
				if(Verbose){ printf("==== Bit number: %d\n", charNumber); };
				break;

			case 'c':
				password = optarg;
				encypt = true;
				flagCrypt = 'y';
				if(Verbose) {
					printf(
						"====\n"
						"==== getopt: Chosen option -c. Changes to %c\n====\n",
						 flagCrypt);
				}
				break;

			case 'v':
				setVerbose();
				break;

			case 'z':
				flagCmprs = 'z';
				Comp = true;
				if(Verbose == true)
					printf("==== getopt: Chosen force compression.\n");
				break;

			case 'x':
				flagCmprs = 'x';
				Decomp = true;
				if(Verbose == true)
					printf("==== getopt: Chosen force decompression.\n");
				break;

			case 'h':
				fprintf(stdout, usage, argv[0]);
				return 0;

			default:
				fprintf(stderr, usage, argv[0]);
				exit(EXIT_FAILURE);
		}
	}

// sprawdzamy czy ktoś nie wybrał jednocześnie kompresji i dekompresji
	if((Comp == true) && (Decomp == true)) {
		fprintf(stderr, "%s:\tCan't compress and decompress at the same time. Aborting.\n", argv[0]);
		fclose(input);
		fclose(output);
		return -1;
	}

// sprawdź, czy input pusty
	if(!fread(&empty, sizeof(char), 1, input) ) {
		fprintf(stderr, "%s:\tCannot operate on an empty file.\n", argv[0]);
		fclose(input);
		fclose(output);
		return -1;
	}
	else
		fseek(input, 0, SEEK_SET);


// zerowy stopień kompresji = przekopiowanie pliku
	if(!charNumber) {
		while(fread(&empty, sizeof(char), 1, input) )
			fwrite(&empty, sizeof(char), 1, output);
		fclose(input);
		fclose(output);
		return 0;
	}

	if(flagCmprs == 'x') {	// forced decomp, sprawdamy czy można
		fseek(input, 2, SEEK_SET);
		fread(&magicNumber, sizeof(char), 1, input);
		if(MagicNum(input, magicNumber, Verbose) != checkmagicNumber) {
			fprintf(stderr, "%s:\tThere was an error reading the file, it is either not compressed or corrupted.\n\t\tTry a different file.\n", argv[0]);
			fclose(input);
			fclose(output);
			return -1;
		}
		if(charNumber != -1) { 			//ręcznie wybrano poziom dekompresji
			fseek(input, 3, SEEK_SET);
			fread(&empty, sizeof(char), 1, input);
			empty >>= 6;
			empty = empty & 0b11;
			if(empty != flagBit) {
				fprintf(stderr, "%s:\tCan't decompress file using %d-bit decompression. Aborting.\n", argv[0], charNumber);
				fclose(input);
				fclose(output);
				return -1;
			}
		}
	}

// sprawdzamy plik wejściowy, jeżeli nie wybrano -x lub -z
	if(flagCmprs == 'n') {
		unsigned char init1, init2, crc;
		fseek(input, 0, SEEK_SET);

		if(	fread(&init1, sizeof(char), 1, input) == 0 ||
			fread(&init2, sizeof(char), 1, input) == 0 ||
			fread(&crc, sizeof(char), 1, input) == 0) {
			flagCmprs = 'z';
			if(Verbose){
				printf(
				"====\n====\n==== CHECKSUM\n"
				"==== Input file analized, it's not a compressed file.\n"
				"==== Chosen compression.\n");
			}
		}
		if(init1 != 'B' || (init2 != 'J' && init2 != 'K') ) {
		// dlaczego init2 może mieć dwie wartości?
		// Wyjaśnione w Flag/flag.c, funkcji addFlag
			flagCmprs = 'z';
			if(Verbose){
				printf(
				"====\n====\n==== CHECKSUM\n"
				"==== Input file analized, it's not a compressed file.\n"
				"==== Chosen compression.\n");
			}
		}
		else {
			if(MagicNum(input, crc, Verbose) == checkmagicNumber) {
				flagCmprs = 'x';
				if(Verbose){
					printf(
					"====\n====\n==== CHECKSUM\n"
					"==== Input file analized, it's a compressed file.\n"
					"==== Chosen decompression.\n");
				}
			}
			else {
				flagCmprs = 'z';
				if(Verbose){
					printf(
					"====\n"
					"====\n"
					"==== CHECKSUM\n"
					"==== Input file analized, it's a decompressed or a corrupted compressed file.\n"
					"==== Chosen compression.\n");
				}
			}
		}
	}


// kompresja
	if(flagCmprs == 'z') {
		char isLast;
		if(Verbose){
			printf(
			"====\n"
			"====\n"
			"==== COMPRESSION\n"
			);
		}

		switch(flagBit) {

// 8-bit kompresja
			case 1:

/* deklaracje zmiennych */
				krokiet_t obiad[256];
				unsigned short ile = 0;
				char cntr = 0;
				unsigned char temp;
				d_t tree = NULL;
				isLast = 0;
				if(Verbose) {
					printf(
						"==== 8-bit compression\n"
						);
				}
/* liczenie wszystkich znaków w input */
				fseek(input, 0, SEEK_SET);
				while((fread(&temp,sizeof(char),1,input)) == 1) {
					tree = add(tree, temp);
				}

				counter(tree, &ile);
				temp = 0;

/* tylko jeden znak w pliku (nieistotnie, czy występuje wielokrotnie) */
				if(ile == 1) {
					addFlag(output, flagBit, encypt, cntr, ile, magicNumber, isLast);
					unsigned char ttemp;
					fwrite(&(tree->znak), sizeof(char), 1, output);
					fseek(input, 0, SEEK_SET);
					while(fread(&temp, sizeof(char), 1, input) ) {
						if(cntr == 8) {
							fwrite(&ttemp, sizeof(char), 1, output);
							ttemp = 0;
							cntr -= 8;
						}
						ttemp <<= 1;
						ttemp += 1;
						cntr++;
					}
					ttemp <<= 8 - cntr;
					fwrite(&ttemp, sizeof(char), 1, output);
					if(Verbose) {
						printf(
							"====\n"
							"==== Detected one character in file: number %d.\n"
							"==== Ouput file has been compressed accordingly.\n"
						, tree->znak
						);
					}

					fseek(output, 0, SEEK_SET);
					addFlag(output, flagBit, encypt, cntr, ile, magicNumber, isLast);

					if(encypt == true) {
						fseek(output, 6, SEEK_SET);
						XOR2(output, charNumber,Verbose, password);
						if(Verbose) {
							printf(
								"====\n"
								"====\n"
								"==== ENCRYPTING FILE\n"
								"==== File has been encrypted.\n"
							);
						}
					}

					magicNumber = MagicNum(output,magicNumber, Verbose);
					fseek(output,2,SEEK_SET);
					fwrite(&magicNumber, sizeof(char), 1, output);
					if(Verbose) {
						printf(
							"====\n"
							"==== Control sum has been added: "
						);
						printBits(magicNumber, 8);
						printf("\n");
						fclose(dump);
					}

					fclose(input);
					fclose(output);
					return 0;
				}

/* co najmniej 2 różne znaki w pliku - standardowa procedura */
				if(Verbose){
					printf(
						"====\n"
						"==== File read.\n"
						"==== Current 'heap', with numerical values,\n"
						"==== has been written to verboseDump.\n"
					);
					fprintf(dump, "\nHeap:\n");
					writeTree(dump, tree, 0);
					fprintf(dump, "\n\n");
				}


/* tworzenie drzewa Huffmana i tablicy alfabetowej */
				tree = makeHTree(tree);
				ile = 0;
				counter(tree, &ile);
				if(Verbose) {
					printf(
						"====\n"
						"==== Created Huffman tree, written to verboseDump\n"
					);
					fprintf(dump, "\nHuffman tree:\n");
					writeTree(dump, tree, 0);
					fprintf(dump, "\n\n");
				}

				addFlag(output, flagBit, encypt, cntr, ile, magicNumber, isLast);
				prepareKrokiet(obiad);
				fillKrokiet(tree, obiad, 0, -2);
				if(Verbose) {
					printf(
						"====\n"
						"==== Created a dictionary, written to verboseDump.\n"
					);
					fprintf(dump, "\nDictionary:\n");
					printKrokiet(dump, obiad);
					fprintf(dump, "\n\n");
				}

/* zakodowanie i zapisanie drzewa */

				codeTree(tree, output, &temp, &cntr);
				if(Verbose) {
					printf(
						"====\n"
						"==== Huffman tree has been coded and written in output file.\n"
					);
				}

/* powrót do początku pliku */
				if(fseek(input, 0, SEEK_SET) ){
					fprintf(stderr, "%s:\tThere was an error while reading the file. Aborting.\n", argv[0]);
					return EXIT_FAILURE;
				}

/* kompresja pliku input */
				if(Verbose) {
					printf(
						"====\n"
						"====\n"
						"==== COMPRESSING FILE\n"
					);
				}
				codeFile(obiad, input, output, &temp, &cntr);
				temp <<= (8 - cntr);
				fwrite(&temp, sizeof(char), 1, output);
				if(Verbose) {
					printf(
						"====\n"
						"==== File has been correctly compressed.\n"
					);
				}

/* dodanie inicjałów oraz właściwych flag do pliku output */
				fseek(output, 0, SEEK_SET);
				addFlag(output, flagBit, encypt, cntr, ile, magicNumber, isLast);

/* xorowanie */
				if(encypt == true) {
					fseek(output, 6, SEEK_SET);
					XOR2(output, charNumber,Verbose, password);
					if(Verbose) {
						printf(
							"====\n"
							"====\n"
							"==== ENCRYPTING FILE\n"
							"==== File has been encrypted.\n"
						);
					}
				}

/* dodanie sumy kontrolnej */
				magicNumber = MagicNum(output,magicNumber, Verbose);
				fseek(output,2,SEEK_SET);
				fwrite(&magicNumber, sizeof(char), 1, output);
				if(Verbose) {
					printf(
						"====\n"
						"==== Control sum has been added: "
					);
					printBits(magicNumber, 8);
					printf("\n");
				}

/* zwolnienie alokowanej pamięci */
				freeTree(tree);
				break;

/* na tej samej zasadzie działają kompresje 12 i 16 bitowe */


// 12-bit kompresja
			case 2:
				if(Verbose) {
					printf(
						"==== 12-bit compression\n"
						);
				}
				fseek(input, 0, SEEK_SET);
		        static krokiet_t12 midObiad[4096];		// bez static za duża struktura danych, daje segfault
				unsigned short temp12;
				short ile12 = 0;
				char cntr12 = 0;
				unsigned char *aaa = malloc(3 * sizeof(char) );
				d_t16 tree12 = NULL;

				isLast = 0;
				while( ( isLast = fread(aaa, sizeof(char), 3, input) ) == 3 ) {
					temp12 = 0;
					temp12 += *aaa;
					temp12 <<= 8;
					temp12 += *(aaa+1);
					temp12 >>= 4;
					temp12 &= 0b0000111111111111;		// wszelki wypadek
					tree12 = add16(tree12, temp12);

					temp12 = 0;
					temp12 += *(aaa+1);
					temp12 <<= 8;
					temp12 += *(aaa+2);
					temp12 &= 0b0000111111111111;		// wszelki wypadek

					tree12 = add16(tree12, temp12);
				}
				if(isLast > 0) {
					temp12 = 0;
					temp12 += *aaa;
					temp12 &= 0b11111111;
					if(isLast == 2) {
						temp12 <<= 8;
						temp12 += *(aaa+1);
						temp12 >>= 4;
						temp12 &= 0b0000111111111111;
						tree12 = add16(tree12, temp12);

						temp12 = 0;
						temp12 += *(aaa+1);
						temp12 &= 0b1111;
						tree12 = add16(tree12, temp12);
					}
					else {
						tree12 = add16(tree12, temp12);
					}
				}

				counter16(tree12, &ile12);
				temp12 = 0;
				addFlag(output, flagBit, encypt, cntr12, ile12, magicNumber, isLast);

/*	tylko jeden 12-bitowy znak w pliku (nieistotnie, czy występuje wielokrotnie).
	Taka sytuacja może wydarzyć się tylko wtedy, gdy każdy bajt pliku input składa
	się z dwóch takich samych 4-bitowych ciągów, np. 00000000, 00110011, 10011001, ...
	W przeciwnym przypadku, nawet jeśli byłby to jeden znak 8-bitowy, będą dwa różne znaki 12-bitowe.
	Przykładowo, w pliku występuje wyłącznie 'a', czyli 01100001.
	Znaki 12-bitowe to 011000010110 i 000101100001. - dwa różne, nie ten przypadek.
*/
				if(ile == 1) {
					unsigned char temp222;
					unsigned short huh;
					fseek(input, 6, SEEK_SET);
					fread(&huh, sizeof(char), 1, input);
					fwrite(&huh, sizeof(char), 1, output);
					fseek(input, 0, SEEK_SET);
					while(fread(&temp222, sizeof(char), 1, input) ) {
						if(cntr12 == 8) {
							fwrite(&temp222, sizeof(char), 1, output);
							temp222 = 0;
							cntr12 -= 8;
						}
						temp222 <<= 1;
						temp222 += 1;
						cntr12++;
					}
					temp222 <<= 8 - cntr12;
					fwrite(&temp222, sizeof(char), 1, output);
					temp222 = huh;
					huh <<= 4;
					huh &= 0b0000111100000000;
					huh += temp222;
					if(Verbose) {
						printf(
							"====\n"
							"==== Detected one character in file: number %d.\n"
							"==== Ouput file has been compressed accordingly.\n"
						, huh
						);
						fclose(dump);
					}
					fclose(input);
					fclose(output);
					return 0;
				}


				if(Verbose){
					printf(
						"====\n"
						"==== File read.\n"
						"==== Current 'heap', with numerical values,\n"
						"==== has been written to verboseDump.\n"
					);
					fprintf(dump, "\nHeap:\n");
					writeTree16(dump, tree12, 0);
					fprintf(dump, "\n\n");
				}
				tree12 = makeHTree16(tree12);
				temp12 = 0;
				ile12 = 0;
				counter16(tree12, &ile12);
				if(Verbose) {
					printf(
						"====\n"
						"==== Created Huffman tree, written to verboseDump\n"
					);
					fprintf(dump, "\nHuffman tree:\n");
					writeTree16(dump, tree12, 0);
					fprintf(dump, "\n\n");
				}

				prepareKrokiet12(midObiad);
				fillKrokiet12(tree12, midObiad, 0, -2);

				if(Verbose) {
					printf(
						"====\n"
						"==== Created a dictionary, written to verboseDump.\n"
					);
					fprintf(dump, "\nDictionary:\n");
					printKrokiet12(dump, midObiad);
					fprintf(dump, "\n\n");
				}

				temp12 = 0;
				cntr12 = 0;
				codeTree12(tree12, output, &temp12, &cntr12);

				if(Verbose) {
					printf(
						"====\n"
						"==== Huffman tree has been coded and written in output file.\n"
					);
				}
				if(fseek(input, 0, SEEK_SET) ){
					fprintf(stderr, "%s:\tThere was an error while reading the file. Aborting.\n", argv[0]);
					return EXIT_FAILURE;
					}
				if(Verbose) {
					printf(
						"====\n"
						"====\n"
						"==== COMPRESSING FILE\n"
					);
				}

				codeFile12(midObiad, input, output, &temp12, &cntr12);

				if(isLast == 1) {
					fseek(input, -1, SEEK_END);
					fread(aaa, sizeof(char), 1, input);
					int jjj = 1;
					while( (midObiad[*aaa].kod[jjj] == 0 || midObiad[*aaa].kod[jjj] == 1 )
							&& midObiad[*aaa].kod[jjj+1] >= 0 ) {
						if(cntr12 == 16) {
							fwrite(&temp12, sizeof(short), 1, output);
							temp12 = 0;
							cntr12 -= 16;
						}
						temp12 <<= 1;
						temp12 += midObiad[*aaa].kod[jjj++];
						cntr12++;
					}
				}
				else if(isLast == 2) {
					fseek(input, -2, SEEK_END);
					fread(aaa, sizeof(char), 2, input);
					int jjj = 1;
					while( (midObiad[*aaa].kod[jjj] == 0 || midObiad[*aaa].kod[jjj] == 1 )
							&& midObiad[*aaa].kod[jjj+1] >= 0 ) {
						if(cntr12 == 16) {
							fwrite(&temp12, sizeof(short), 1, output);
							temp12 = 0;
							cntr12 -= 16;
						}
						temp12 <<= 1;
						temp12 += midObiad[*aaa].kod[jjj++];
						cntr12++;
					}
					while( (midObiad[*(aaa+1)].kod[jjj] == 0 || midObiad[*(aaa+1)].kod[jjj] == 1 )
							&& midObiad[*(aaa+1)].kod[jjj+1] >= 0 ) {
						if(cntr12 == 16) {
							fwrite(&temp12, sizeof(short), 1, output);
							temp12 = 0;
							cntr12 -= 16;
						}
						temp12 <<= 1;
						temp12 += midObiad[*(aaa+1)].kod[jjj++];
						cntr12++;
					}
				}

				temp12 <<= (16 - cntr12);
				fwrite(&temp12, sizeof(short), 1, output);
				if(Verbose) {
					printf(
						"====\n"
						"==== File has been correctly compressed.\n"
					);
				}


				fseek(output, 0, SEEK_SET);
				addFlag(output, flagBit, encypt, cntr12, ile12, magicNumber, isLast);

				if(encypt == true) {
					if(Verbose) {
						printf(
							"====\n"
							"====\n"
							"==== ENCRYPTING FILE\n"
						);
					}
					fseek(output, 6, SEEK_SET);
					XOR2(output, charNumber,Verbose, password);
					if(Verbose) {
						printf(
							"====\n"
							"==== File has been encrypted.\n"
						);
					}
				}

				magicNumber = MagicNum(output,magicNumber, Verbose);
				fseek(output,2,SEEK_SET);
				fwrite(&magicNumber, sizeof(char), 1, output);
				if(Verbose) {
					printf(
						"====\n"
						"==== Control sum has been added: "
					);
					printBits(magicNumber, 8);
					printf("\n");
				}

				free(aaa);
				freeTree16(tree12);
				break;




// 16-bit kompresja
			case 3:
				unsigned short ile16 = 0;
				char cntr16 = 0;
				unsigned short temp16;
				unsigned short checkForLast;
				/*	dla sprawdzenia parzystości bajtów
					pliku wejściowego (da się inaczej,
					ale ostatni bajt i tak będzie istotny */

				isLast = 0;
				d_t16 tree16 = NULL;
				krokiet_t16 *bigObiad = NULL;

				if(Verbose) {
					printf(
						"==== 16-bit compression\n"
						);
				}

				fseek(input, 0, SEEK_SET);
				while((fread(&temp16,sizeof(short),1,input)) == 1) {
					tree16 = add16(tree16,temp16);
					checkForLast = temp16;
				}

				if(checkForLast != temp16) {	// fread wczytał jeden bajt do temp16, i zwrócił 0
					tree16 = add16(tree16, temp16);
					checkForLast = temp16;
					isLast = 1;
				}

				counter16(tree16, &ile16);
				temp16 = 0;

/*	tutaj przypadek jednego znaku 16-bitowego jest taki sam, jak dla 8-bit,
	tylko znak jest 2x dłuższy - dowolna powtarzająca się w pliku
	kombinacja dwóch 8-bitowych znaków kwalifikuje plik jako jednoznakowy */
				if(ile16 == 1) {
					cntr16 = 0;
					char temp = 0;
					addFlag(output, flagBit, encypt, cntr16, ile16, magicNumber, isLast);
					fwrite(&(tree16->znak), sizeof(short), 1, output);
					fseek(input, 0, SEEK_SET);
					while(fread(&temp16, sizeof(short), 1, input) ) {
						if(cntr16 == 8) {
							fwrite(&temp, sizeof(char), 1, output);
							temp = 0;
							cntr16 -= 8;
						}
						temp <<= 1;
						temp += 1;
						cntr16++;
					}
					temp <<= 8 - cntr16;
					fwrite(&temp, sizeof(char), 1, output);
					if(Verbose) {
						printf(
							"====\n"
							"==== Detected one character in file: number %d.\n"
							"==== Ouput file has been compressed accordingly.\n"
						, tree16->znak
						);
					}

					fseek(output, 0, SEEK_SET);
					addFlag(output, flagBit, encypt, cntr16, ile16, magicNumber, isLast);

					if(encypt == true) {
						fseek(output, 6, SEEK_SET);
						XOR2(output, charNumber,Verbose, password);
						if(Verbose) {
							printf(
								"====\n"
								"====\n"
								"==== ENCRYPTING FILE\n"
								"==== File has been encrypted.\n"
							);
						}
					}

					magicNumber = MagicNum(output,magicNumber, Verbose);
					fseek(output,2,SEEK_SET);
					fwrite(&magicNumber, sizeof(char), 1, output);
					if(Verbose) {
						printf(
							"====\n"
							"==== Control sum has been added: "
						);
						printBits(magicNumber, 8);
						printf("\n");
						fclose(dump);
					}

					fclose(input);
					fclose(output);
					return 0;
				}




				if(Verbose){
					printf(
						"====\n"
						"==== File read.\n"
						"==== Current 'heap', with numerical values,\n"
						"==== has been written to verboseDump.\n"
					);
					fprintf(dump, "Heap:\n");
					writeTree16(dump, tree16, 0);
					fprintf(dump, "\n\n");
				}

				tree16 = makeHTree16(tree16);
//				temp16 = 0;
				ile16 = 0;
//				cntr16 = 0;
				counter16(tree16, &ile16);
				if(Verbose) {
					printf(
						"====\n"
						"==== Created Huffman tree, written to verboseDump\n"
					);
					fprintf(dump, "\nHuffman tree:\n");
					writeTree16(dump, tree16, 0);
					fprintf(dump, "\n\n");
				}

				addFlag(output, flagBit, encypt, cntr16, ile16, magicNumber, isLast);

                bigObiad = malloc(ile16 * sizeof(krokiet_t16));
                prepareKrokiet16(bigObiad, ile16);
				unsigned short whichDone = 0;		// aby nie alokować 4GB pamięci(2^24 char + 2^12 char)
				fillKrokiet16(tree16, bigObiad, 0, -2, ile16, &whichDone);

				if(Verbose) {
					printf(
						"====\n"
						"==== Created a dictionary, written to verboseDump.\n"
					);
					fprintf(dump, "\nDictionary:\n");
					printKrokiet16(dump, bigObiad, ile16);
					fprintf(dump, "\n\n");
				}

//				cntr16 = 0;
				codeTree16(tree16, output, &temp16, &cntr16);
				if(Verbose) {
					printf(
						"====\n"
						"==== Huffman tree has been coded and written in output file.\n"
					);
				}


				if(fseek(input, 0, SEEK_SET) ){
					fprintf(stderr, "%s:\tThere was an error while reading the file. Aborting.\n", argv[0]);
					freeTree16(tree16);
					return EXIT_FAILURE;
				}

				if(Verbose) {
					printf(
						"====\n"
						"====\n"
						"==== COMPRESSING FILE\n"
					);
				}

				codeFile16(bigObiad, input, output, &temp16, &cntr16);

				if(isLast) {
					int k = 0;
					while( checkForLast != (bigObiad + k)->znak) {
						k++;
					}
    	    	    int j = 1;
					while( ( (bigObiad+k)->kod[j] == 0) || ( (bigObiad+k)->kod[j] == 1) && (bigObiad+k)->kod[j+1] >= 0 ) {
		                if(cntr16 == 16) {
							fwrite(&temp16, sizeof(short), 1, output);
							temp16 = 0;
							cntr16 -= 16;
						}
						temp16 <<= 1;
						temp16 += (bigObiad+k)->kod[j++];
						cntr16++;
					}
				}
				if(cntr16) {
					temp16 <<= (16 - cntr16);
					fwrite(&temp16, sizeof(short), 1, output);
				}
				if(Verbose) {
					printf(
						"====\n"
						"==== File has been correctly compressed.\n"
					);
				}

				fseek(output, 0, SEEK_SET);
				addFlag(output, flagBit, encypt, cntr16, ile16, magicNumber, isLast);


				if(encypt == true) {
					fseek(output, 6, SEEK_SET);
					XOR2(output, charNumber,Verbose, password);
					if(Verbose) {
						printf(
							"====\n"
							"====\n"
							"==== ENCRYPTING FILE\n"
							"==== File has been encrypted.\n"
						);
					}
				}


				magicNumber = MagicNum(output,magicNumber, Verbose);
				fseek(output,2,SEEK_SET);
				fwrite(&magicNumber, sizeof(char), 1, output);
				if(Verbose) {
					printf(
						"====\n"
						"==== Control sum has been added: "
					);
					printBits(magicNumber, 8);
					printf("\n");
				}
				free(bigObiad);
				freeTree16(tree16);
				break;


			default:
				fprintf(stderr, "Natrafił się jakiś bardzo dziwny błąd, tego nie powinno\n"
								"być widać, w ogóle, bo nie mamy piątej opcji dekomrpesji.\n"
								"Możliwe, że program musi odpocząć, proszę mu dać chwilkę.\n"
								" ~JM\n");
				fclose(input);
				fclose(output);
				if(Verbose)
					fclose(dump);
				return -1;
				break;				// dla zasady :)
		}
	}




// dekompresja
	else {

		if(Verbose){
			printf(
				"====\n"
				"====\n"
				"==== DECOMPRESSION\n"
			);
		}

/* deklaracje paru zmiennych */

		int dlugosc;
		short liscie;
		unsigned char temp = 0;
		char last, charNumber = 0, Flag = 0, crc = 0;
		char isLast = 0;



/* sprawdzanie flagi, sumy kontrolnej i <hasła> pliku */

		checkFlag(input, &crc, &Flag, &liscie, Verbose);

		unsigned char tre = Flag;
		tre = tre & 0b11000000;
		tre >>= 6;
		switch(tre) {
			case 1:
				charNumber = 8;
				break;
			case 2:
				charNumber = 12;
				break;
			case 3:
				charNumber = 16;
				break;
			default:
				charNumber = 8;
				break;
		}

		if(Flag & 0b00010000) {
			fseek(input, 1, SEEK_SET);
			fread(&temp, sizeof(char), 1, input);
			if(temp == 'K')
				isLast = 2;
			else
				isLast == 'J';
		}

		if(Flag & 0b00100000) {
			if(flagCrypt == 'y') {
				fseek(input, 6, SEEK_SET);
				XOR2(input, charNumber,Verbose, password);
			} else {
				printf("Password required!\n");
			}
		}
		if(Verbose) {
			printf(
			"====\n"
			"==== %d-bit decompression\n"
			, charNumber
			);
		}



 		switch (charNumber) {


// 8-bit dekompresja
			case 8:

/* deklaracja zmiennych */
				if(liscie == 1) {
					int i, j, liczba;
					unsigned char tremp;
					unsigned char *buf = malloc(100 * sizeof(*buf) );
					last = Flag & 0b00001111;
					fseek(input, 6, SEEK_SET);
					fread(&tremp, sizeof(char), 1, input);
					liczba = fread(buf, sizeof(char), 100, input);
					while(liczba) {
						for( i = 0; i < liczba-1; i++ ){
							for(j = 0; j < 8; j++)
								fwrite(&tremp, sizeof(char), 1, output);
						}

						if(liczba = fread(buf, sizeof(char), 100, input) ) {		// są dalsze dane - nie ostatni bajt
							for(j = 0; j < 8; j++)
								fwrite(&tremp, sizeof(char), 1, output);
						}
						else {
							while(last) {
								fwrite(&tremp, sizeof(char), 1, output);
								last--;
							}
						}
					}
					free(buf);
				}
				else {
					union eitbit trempe;
					char cntr = 8;
					d_t ntree = NULL, lastTree;

					fseek(input, 6, SEEK_SET);
					last = Flag & 0b00001111;

					fread(&(trempe.A), sizeof(char), 1, input);
					fread(&(trempe.B), sizeof(char), 1, input);

	/* odtworzenie drzewa z pliku */

					ntree = readTree(input, &liscie, &trempe, &cntr);

					if(Verbose) {
						printf(
							"====\n"
							"==== Huffman tree has been remade from code,\n"
							"==== written to verboseDump\n"
						);
						fprintf(dump, "\nHuffman tree:\n");
						writeTree(dump, ntree, 0);
						fprintf(dump, "\n\n");
					}

	/* dekodowanie pozostałych bajtów pliku -
	   faktycznej zawartości pliku pierwotnego */

					lastTree = decodeFile(ntree, input, output, &trempe, &cntr);

	/* dekodowanie ostatniego bitu,
	   który może być tylko częściowo wykorzystany */

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
					freeTree(ntree);
				}

				if(Verbose) {
					printf(
						"====\n"
						"==== Input file has been successfully decompressed into output file.\n"
					);
				}

/* zwalnianie pamięci */

				break;

/* 	pozostałe dekompresje działają na tej samej zasadzie,
	wyłącznie z innymi wielkościami 'znaku' oraz różnicą
	w ostatnich bajtach pliku
*/


// 12-bit dekompresja
			case 12:
				printf("work in progress\n");

				if(liscie == 1) {
					int i, j, liczba;
					unsigned char tremp;
					unsigned char *buf = malloc(100 * sizeof(*buf) );
					char cntr = 8;
					fread(&tremp, sizeof(char), 1, input);
					liczba = fread(buf, sizeof(char), 100, input);
					last = Flag & 0b00001111;
					while(liczba) {
						for( i = 0; i < liczba-1; i++ ){
							for(j = 0; j < 8; j++)
								fwrite(&tremp, sizeof(char), 1, output);
						}

						if(liczba = fread(buf, sizeof(char), 100, input) ) {		// są dalsze dane - nie ostatni bajt
							for(j = 0; j < 8; j++)
								fwrite(&tremp, sizeof(char), 1, output);
						}
						else {
							while(last) {
								fwrite(&tremp, sizeof(char), 1, output);
								last--;
							}
						}
					}
					free(buf);
				}

				else {
					union sixtbit twlv;
					char cntr12 = 0;
					d_t16 ntree12 = NULL, lastTree12;

					fseek(input, 6, SEEK_SET);
					last = Flag & 0b00001111;

					fread(&(twlv.C), sizeof(short), 1, input);
					fread(&(twlv.D), sizeof(short), 1, input);

					cntr12 = 16;

					ntree12 = readTree12(input, &liscie, &twlv, &cntr12);


					if(Verbose) {
						printf(
							"====\n"
							"==== Huffman tree has been remade from code,\n"
							"==== written to verboseDump.\n"
						);
						fprintf(dump, "\nHuffman tree:\n");
						writeTree16(dump, ntree12, 0);
						fprintf(dump, "\n\n");
					}
	/*
	/* dekodowanie pozostałych bajtów pliku -
	   faktycznej zawartości pliku pierwotnego */
					char whether = 0;
					unsigned char outchar = 0;
					lastTree12 = decodeFile12(ntree12, input, output, &twlv, &cntr12, &whether, &outchar);

	/* dekodowanie ostatniego bitu,
	   który może być częściowo wykorzystany */

					last+=cntr12;
					unsigned short roon;				//ran out of names
	// po tym sporo do zmiany
					while(last) {
						if(lastTree12->counter) {
							if(!whether) {
								roon = lastTree12->znak;
								roon >>= 4;
								roon &= 0b0000000011111111;
								outchar = roon;
								fwrite( &(outchar), sizeof(char), 1, output);
								outchar = 0;
								roon = lastTree12->znak;
								roon &= 0b0000000000001111;
								outchar = roon;
								whether = 1;
							}
							else {
								roon = lastTree12->znak;
								roon >>= 8;
								roon &= 0b0000000000001111;
								outchar <<= 4;
								outchar += roon;
								fwrite( &outchar, sizeof(char), 1, output);
								outchar = 0;
								roon = lastTree12->znak;
								roon &= 0b0000000011111111;
								outchar = roon;
								fwrite( &outchar, sizeof(char), 1, output);
								outchar = 0;
								whether = 0;
							}
							lastTree12 = ntree12;
						}
						if(!bit2(twlv.C, 15) ) {
							twlv.cd <<=1;
							last--;
							lastTree12 = lastTree12->left_node;
						}
						else {
							twlv.cd <<=1;
							last--;
							lastTree12 = lastTree12->right_node;
						}
					}
					if(!whether) {
						roon = lastTree12->znak;
						roon >>= 4;
						roon &= 0b0000000011111111;
						outchar = roon;
						fwrite( &(outchar), sizeof(char), 1, output);
						outchar = 0;
						roon = lastTree12->znak;
						roon <<= 4;
						roon &= 0b0000000011110000;
						outchar = roon;
						whether = 1;
					}
					else {
						roon = lastTree12->znak;
						roon >>= 8;
						roon &= 0b0000000000001111;
						outchar += roon;
						fwrite( &outchar, sizeof(char), 1, output);
						outchar = 0;
						roon = lastTree12->znak;
						roon &= 0b0000000011111111;
						outchar = roon;
						fwrite( &outchar, sizeof(char), 1, output);
						outchar = 0;
						whether;
					}

					if(Verbose) {
						printf(
							"====\n"
							"==== %s has been successfully decompressed into %s.\n"
							, argv[argc-2], argv[argc-1]
						);
					}

	/* możliwość nieparzystego bajta w oryginalnym pliku */
	/*
					if(isLast) {
						if(Verbose) {
							printf(
								"====\n"
								"==== Detected an additional byte - removing...\n"
							);
						}
						int addition = -1;
						unsigned char addit;
						FILE *additional = fopen(".mysteriousFile", "wb+");
						fseek(output, 0, SEEK_SET);
						while(fread(&addit, sizeof(char), 1, output) ) {
							fwrite(&addit, sizeof(char), 1, additional);
							addition++;
						}
						fclose(output);
						output = fopen(argv[argc-1], "wb");
						fseek(additional, 0, SEEK_SET);
						while(addition) {
							fread(&addit, sizeof(char), 1, additional);
							fwrite(&addit, sizeof(char), 1, output);
							addition--;
						}
						if(Verbose) {
							printf(
								"====\n"
								"==== Additional byte successfully removed.\n"
							);
						}
						fclose(additional);
					}
	*/
				freeTree16(ntree12);
				}
			// tutej jeszcze verbose
				break;



// 16-bit dekompresja
			case 16:
				if(liscie == 1) {
					int i, j, liczba;
					unsigned short tremp;
					unsigned char *buf = malloc(100 * sizeof(*buf) );
					fseek(input, 6, SEEK_SET);
					fread(&tremp, sizeof(short), 1, input);
					liczba = fread(buf, sizeof(char), 100, input);
					last = Flag & 0b00001111;
					while(liczba) {
						for( i = 0; i < liczba-1; i++ ){
							for(j = 0; j < 8; j++)
								fwrite(&tremp, sizeof(short), 1, output);
						}

						if(liczba = fread(buf, sizeof(char), 100, input) ) {		// są dalsze dane - nie ostatni bajt
							for(j = 0; j < 8; j++)
								fwrite(&tremp, sizeof(short), 1, output);
						}
						else {
							while(last) {
								fwrite(&tremp, sizeof(short), 1, output);
								last--;
							}
						}
					}
					free(buf);
					if(Verbose) {
						printf(
							"====\n"
							"==== Succesfully decompressed file.\n"
						);
					}
					fclose(input);
					fclose(output);
					return 0;
				}

				else {
					union sixtbit sixsaas;
//					unsigned short temp16 = 0;
					char cntr16 = 16;
					d_t16 ntree16 = NULL, lastTree16;

					isLast = 0;
					last = Flag & 0b00001111;

					isLast = Flag & 0b00010000;

					fseek(input, 6, SEEK_SET);

					fread(&(sixsaas.C), sizeof(short), 1, input);
					fread(&(sixsaas.D), sizeof(short), 1, input);
					ntree16 = readTree16(input, &liscie, &sixsaas, &cntr16);


					if(Verbose) {
						printf(
							"====\n"
							"==== Huffman tree has been remade from code,\n"
							"==== written to verboseDump.\n"
						);
						fprintf(dump, "\nHuffman tree:\n");
						writeTree16(dump, ntree16, 0);
						fprintf(dump, "\n\n");
					}
	/* dekodowanie pozostałych bajtów pliku -
	   faktycznej zawartości pliku pierwotnego */

					lastTree16 = decodeFile16(ntree16, input, output, &sixsaas, &cntr16);

	/* dekodowanie ostatniego bitu,
	   który może być częściowo wykorzystany */
					last+=cntr16;
					while(last) {
						if(lastTree16->counter) {
							fwrite( &(lastTree16->znak), sizeof(short), 1, output);
							lastTree16 = ntree16;
						}
						if(!bit2(sixsaas.C, 15) ) {
							sixsaas.cd <<=1;
							last--;
							lastTree16 = lastTree16->left_node;
						}
						else {
							sixsaas.cd <<=1;
							last--;
							lastTree16 = lastTree16->right_node;
						}
					}
					fwrite(&(lastTree16->znak), sizeof(short), 1, output);
					if(Verbose) {
						printf(
							"====\n"
							"==== %s has been successfully decompressed into %s.\n"
							, argv[argc-2], argv[argc-1]
						);
					}
	/* możliwość nieparzystego bajta w oryginalnym pliku */

					if(isLast) {
							if(Verbose) {
							printf(
								"====\n"
								"==== Detected an additional byte - removing...\n"
							);
						}
						int addition = -1;
						unsigned char addit;
						FILE *additional = tmpfile();
						if(additional == NULL) {
							fprintf(stderr, "%s:\tSomething's wrong, can't open a temp file. Aborting.\n", argv[0]);
							return -1;
						}
						fseek(output, 0, SEEK_SET);
						while(fread(&addit, sizeof(char), 1, output) ) {
							fwrite(&addit, sizeof(char), 1, additional);
							addition++;
						}
						fclose(output);
						output = fopen(argv[argc-1], "wb");
						fseek(additional, 0, SEEK_SET);
						while(addition) {
							fread(&addit, sizeof(char), 1, additional);
							fwrite(&addit, sizeof(char), 1, output);
							addition--;
						}
						if(Verbose) {
							printf(
								"====\n"
								"==== Additional byte successfully removed.\n"
							);
						}
						fclose(additional);
					}
					freeTree16(ntree16);
				}

				break;


			default:

				break;

		}
	}

	fclose(input);
	fclose(output);
	if(Verbose) {
		printf(
			"====\n"
			"==== Program exited correctly.\n"
		);
		fclose(dump);
	}
	fprintf(stdout, "%s:\tDone doing things.\n", argv[0]);
	return 0;
}
