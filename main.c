#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

//#include "szyfr.h"

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

int main(int argc, char **argv) {
	char flagComp = 'n', flagCrypt = 'n', flagVerb = 'n';
	int flagBit = 0;
	int opt;
	if(argc < 2){
		fprintf(stderr, "%s: Not enough arguments!\n\n%s\n", argv[0], usage);
		return 1;
	}
	while ( (opt = getopt (argc, argv, "o:cvhxz") ) != -1 ) {
// w tym printy fo usunięcia / może dać gdy verbose?
		switch(opt) {
			case 'o':
				flagBit = atoi (optarg);
				printf("Chosen option -o %d\n", flagBit);
				break;

			case 'c':
				flagCrypt = 'y';
				printf("Chosen option -c. Changes to %c\n", flagCrypt);
				break;
			case 'v':
				flagVerb = 'y';
				printf("Chosen option -v.\n");
				break;
			case 'z':
				flagComp = 'c';
				printf("Chosen force compression\n");
				break;
			case 'x':
				flagComp = 'd';
				printf("Chosen force decompression\n");
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
