#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "szyfr.h"

int main(int argc, char ** argv)
{
    srand(time(NULL));
    FILE *input = argc > 1 ? fopen(argv[1], "rb") : stdin;
    FILE *output = argc > 2 ? fopen (argv[2], "wb") : stdin;

    if(input == NULL) {
        printf("ERROR1\n");
        return -1;
    }
    int num = 12;   //ile bitów
//    XOR(input, output, num, false, "abc");
//#ifdef DEBUG
//        input = fopen(argv[1], "rb");
//        output = fopen(argv[2], "rb");
//        print_files(input, output);
//#endif
    unsigned char magicnumber = 69;
    printf("%d(%c)->", magicnumber, magicnumber);
    magicnumber = MagicNum(input,magicnumber);
    printf("%d(%c)->", magicnumber, magicnumber);
    fclose(input);
    input = fopen(argv[1], "rb");
    printf("%d(%c)\n",MagicNum(input, magicnumber),MagicNum(input, magicnumber));
    fclose(output);
    return 0;
}