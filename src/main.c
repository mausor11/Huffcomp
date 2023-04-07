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
			break;
		}
	}

	if(Verbose) {
		printf("==== Huffman coding\n");
		printf("==== Copyright (C), by Bartosz Dańko and Jan Machowski\n");
		printf("==== Command: ");
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
					printf("====\n");
					printf("==== getopt: Chosen option -c. Changes to %c\n", flagCrypt);
					printf("====\n");
				}
				break;

			case 'v':
                setVerbose();
				break;

			case 'z':
				flagCmprs = 'z';
                Comp = true;
				if(Verbose == true)
					printf("==== getopt: Chosen force compression\n");
				break;

			case 'x':
				flagCmprs = 'x';
                Decomp = true;
				if(Verbose == true)
					printf("==== getopt: Chosen force decompression\n");
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
			fprintf(stderr, "%s:\tThere was an error reading the file,\n\tit is possibly corrupted.\n\tTry a different file.\n", argv[0]);
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
				"====\n==== CHECKSUM\n"
				"==== Input file analized, it's not a compressed file.\n"
				"==== Chosen compression.\n");
			}
		}
		if(init1 != 'B' || init2 != 'J') {
			flagCmprs = 'z';
			if(Verbose){
				printf(
				"====\n==== CHECKSUM\n"
				"==== Input file analized, it's not a compressed file.\n"
				"==== Chosen compression.\n");
			}
		}
		else {
			if(MagicNum(input, crc, Verbose) == checkmagicNumber) {
				flagCmprs = 'x';
				if(Verbose){
					printf(
					"====\n==== CHECKSUM\n"
					"==== Input file analized, it's a compressed file.\n"
					"==== Chosen decompression.\n");
				}
			}
			else {
				flagCmprs = 'z';
				if(Verbose){
					printf(
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
			"==== COMPRESSION\n"
			"==== %d-bit compression\n"
			, charNumber
			);
		}
// switch:case poziomu kompresji
    switch(flagBit) {

		case 2: 
		unsigned char tempBajt;
		int ileBajtow = 0;
		fseek(input, 0, SEEK_SET);
		while(fread(&tempBajt, sizeof(char), 1, input) )
			ileBajtow++;
		fseek(input, 0, SEEK_SET);
		printf("Plik ma %d bajtów.\n", ileBajtow);
/*
        krokiet_t12 obiad[4096];
        short ile = 0;
        char cntr = 0;
        unsigned char temp;
        d_t tree = NULL;


            fseek(input, 0, SEEK_SET);
            while((fread(&temp,sizeof(char),1,input)) == 1) {
                tree = add(tree, temp);
            }
        if(Verbose){
            printf(
                "====\n"
                "==== File read.\n"
                "==== Current 'heap', with numerical values;\n"
                );
            writeTree(tree, 0);
            }

            tree = makeHTree16(tree);
            temp = 0;
            counter(tree, &ile);
            if(Verbose) {
                printf(
                        "====\n"
                        "==== Created Huffman tree:\n"
                );
                writeTree(tree, 0);
            }

            addFlag(output, flagBit, encypt, cntr, ile, magicNumber);

            prepareKrokiet(obiad);
            fillKrokiet(tree, obiad, 0, -2);

            if(Verbose) {
                printf(
                        "====\n"
                        "==== Created a dictionary:\n"
                );
                printKrokiet(obiad);
            }


            codeTree(tree, output, &temp, &cntr);

            if(Verbose) {
                printf(
                        "====\n"
                        "==== Huffman tree has been coded and written in output file.\n"
                );
            }

            if(fseek(input, 0, SEEK_SET) ){
                fprintf(stderr, "%s:\tThere was an error while reading the file.\n\tAborting.\n", argv[0]);
                return EXIT_FAILURE;
            }


            codeFile(obiad, input, output, &temp, &cntr);
            temp <<= (8 - cntr);
            fwrite(&temp, sizeof(char), 1, output);
            if(Verbose) {
                printf(
                        "====\n"
                        "==== COMPRESSING FILE\n"
                        "==== File has been correctly compressed.\n"
                );
            }


            fseek(output, 0, SEEK_SET);
            addFlag(output, flagBit, encypt, cntr, ile, magicNumber);


            if(encypt == true) {
                fseek(output, 6, SEEK_SET);
                XOR2(output, charNumber,Verbose, password);
                if(Verbose) {
                    printf(
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

				freeTree(tree);
*/
				break;


    case 3:
/* deklaracje zmiennych */
        short ile16 = 0;
        char cntr16 = 0;
        unsigned short temp16;
       	unsigned short checkForLast;		// dla sprawdzenia parzystości bajtów pliku wejściowego
		isLast = 0;
	   	d_t16 tree16 = NULL;
        krokiet_t16 *bigObiad = NULL;

/* liczenie wszystkich znaków w input */
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

        if(Verbose){
            printf(
                "====\n"
                "==== File read.\n"
                "==== Current 'heap', with numerical values;\n"
            );
                writeTree16(tree16, 0);
        }

/* tworzenie drzewa huffmana i tablicy alfabetowej*/
        tree16 = makeHTree16(tree16);
        temp16 = 0;
        counter16(tree16, &ile16);
        if(Verbose) {
                printf(
                        "====\n"
                        "==== Created Huffman tree:\n"
                );
            writeTree16(tree16, 0);
        }
            addFlag(output, flagBit, encypt, cntr16, ile16, magicNumber, isLast);

		bigObiad = malloc(ile16 * sizeof(krokiet_t16));
            prepareKrokiet16(bigObiad, ile16);
            short whichDone = 0;
            fillKrokiet16(tree16, bigObiad, 0, -2, ile16, &whichDone);
        if(Verbose) {
                printf(
                        "====\n"
                        "==== Created a dictionary:\n"
                );
                printKrokiet16(bigObiad, ile16);
            }


            codeTree16(tree16, output, &temp16, &cntr16);

            if(Verbose) {
                printf(
                        "====\n"
                        "==== Huffman tree has been coded and written in output file.\n"
                );
            }



            if(fseek(input, 0, SEEK_SET) ){
                fprintf(stderr, "%s:\tThere was an error while reading the file.\n\tAborting.\n", argv[0]);
				freeTree16(tree16);
                return EXIT_FAILURE;
            }

            if(Verbose) {
                printf(
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


/* deklaracje zmiennych */
        krokiet_t obiad[256];
        short ile = 0;
        char cntr = 0;
        unsigned char temp;
        d_t tree = NULL;
		isLast = 0;

/* liczenie wszystkich znaków w input */
            fseek(input, 0, SEEK_SET);
            while((fread(&temp,sizeof(char),1,input)) == 1) {
                tree = add(tree, temp);
            }
        if(Verbose){
            printf(
                "====\n"
                "==== File read.\n"
                "==== Current 'heap', with numerical values;\n"
                );
            writeTree(tree, 0);
            }
/* tworzenie drzewa huffmana i tablicy alfabetowej*/
            tree = makeHTree(tree);
            temp = 0;
            counter(tree, &ile);
            if(Verbose) {
                printf(
                        "====\n"
                        "==== Created Huffman tree:\n"
                );
                writeTree(tree, 0);
            }

            addFlag(output, flagBit, encypt, cntr, ile, magicNumber, isLast);

            prepareKrokiet(obiad);
            fillKrokiet(tree, obiad, 0, -2);

            if(Verbose) {
                printf(
                        "====\n"
                        "==== Created a dictionary:\n"
                );
                printKrokiet(obiad);
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
                fprintf(stderr, "%s:\tThere was an error while reading the file.\n\tAborting.\n", argv[0]);
                return EXIT_FAILURE;
            }


/* kompresja pliku input */
            codeFile(obiad, input, output, &temp, &cntr);
            temp <<= (8 - cntr);
            fwrite(&temp, sizeof(char), 1, output);
            if(Verbose) {
                printf(
                        "====\n"
                        "==== COMPRESSING FILE\n"
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
        }
	}

// dekompresja
	else {
		char isLast = 0;

		if(Verbose){
			printf(
			"====\n"
			"==== DECOMPRESSION\n"
			);
		}

/* deklaracje zmiennych */

		int dlugosc;
		short liscie;
		char cntr = 8, last, flag, charNumber = 0, Flag = 0, crc = 0;
		unsigned char temp = 0;

        char cntr16 = 16;
        unsigned short temp16 = 0;
        d_t16 ntree16 = NULL, lastTree16;




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

		if(Flag & 0b00010000)
			isLast = 1;

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



// begin decompress
    switch (charNumber) {
            case 12:
                break;




            case 16:
		        union sixtbit sixsaas;
				d_t16 ntree16 = NULL, lastTree16;

				fseek(input, 6, SEEK_SET);
	            last = Flag & 0b00001111;

	            fread(&(sixsaas.C), sizeof(short), 1, input);
	            fread(&(sixsaas.D), sizeof(short), 1, input);

	            ntree16 = readTree16(input, &liscie, &sixsaas, &cntr16);


	            if(Verbose) {
	                printf(
	                        "====\n"
	                        "==== Huffman tree has been remade from code:\n"
	                );
	                writeTree16(ntree16, 0);
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
	                        "==== Input file has been successfully decompressed into output file.\n"
	                );
	            }

/* możliwość nieparzystego bajta */

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

/* zwalnianie pamięci */

				freeTree16(ntree16);
				break;



            default:
/* odtworzenie drzewa z pliku */
				union eitbit trempe;
				d_t ntree = NULL, lastTree;

	            fseek(input, 6, SEEK_SET);
	            last = Flag & 0b00001111;

	            fread(&(trempe.A), sizeof(char), 1, input);
				fread(&(trempe.B), sizeof(char), 1, input);

        	    ntree = readTree(input, &liscie, &trempe, &cntr);

        	    if(Verbose) {
					printf(
						"====\n"
						"==== Huffman tree has been remade from code:\n"
					);
                	writeTree(ntree, 0);
            	}

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

	            if(Verbose) {
	                printf(
	                        "====\n"
	                        "==== Input file has been successfully decompressed into output file.\n"
	                );
	            }

/* zwalnianie pamięci */

	            freeTree(ntree);
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
	}
	return 0;
}
