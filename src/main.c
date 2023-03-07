#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>

#include "SzyfrXOR/szyfr.h"



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
	"		* -v - print additional info into stdout;\n"
	"		* -h - print help.\n";

bool Verbose = false;

void setVerbose () {
    Verbose = true;
}

int main(int argc, char **argv) {
	srand(time(NULL));
	char flagComp = 'n', flagCrypt = 'n', flagVerb = 'n';
	int flagBit = 0;
	int opt;
	if(argc < 2){
		fprintf(stderr, "%s: Not enough arguments!\n\n%s\n", argv[0], usage);
		return 1;
	}
    if(argc < 3){
        fprintf(stderr, "%s: Not enough arguments to open file!\n\n%s\n", argv[0], usage);
        return 1;
    }

        FILE *input = fopen(argv[argc-2], "r");
        FILE *output = fopen(argv[argc-1], "w");
        printf("->%s\n", argv[argc-1]);
        if(input == NULL) {
            fprintf(stderr, "%s: File %s is not open!\n",argv[0], argv[argc-2]);
            return -1;
        } else if(output == NULL) {
            fprintf(stderr, "%s: FILE %s is not open!\n",argv[0], argv[argc-1]);
            return -1;
        }

	int char_number = 8;

        for(int i=1;i<argc;i++) {
            if(strcmp(argv[i], "-v") == 0) {
                printf("verbose = true\n");
                setVerbose();
                break;
            }
        }

	while ( (opt = getopt (argc, argv, "o:cvhxz") ) != -1 ) {
// w tym printy fo usunięcia / może dać gdy verbose?
		switch(opt) {
			case 'o':
				flagBit = atoi (optarg);
                if(Verbose == true) {
                    printf("Chosen option -o %d\n", flagBit);
                }

				if(flagBit == 1) {
					char_number = 8;
				} else if(flagBit == 2) {
					char_number = 12;
				} else if(flagBit == 3) {
					char_number = 16;
				} else {
					fprintf(stderr, "%s: Wrong option!\n\n%s\n", argv[0], usage);
				}
				break;
			case 'c':
				flagCrypt = 'y';
                if(Verbose == true) {
                    printf("Chosen option -c. Changes to %c\n", flagCrypt);
                    printf("bit: %d\n", char_number);
                }
                		XOR(input, output, char_number, Verbose);
				break;
			case 'v':
//				flagVerb = 'y';
//                if(Verbose == true)
//                {
//                    printf("Chosen option -v.\n");
//                }
//                setVerbose();
				break;
			case 'z':
				flagComp = 'c';
                if(Verbose == true) {
                    printf("Chosen force compression\n");
                }
				break;
			case 'x':
				flagComp = 'd';
                if(Verbose == true) {
                    printf("Chosen force decompression\n");
                }
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
