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
    XOR(input, output, num, false, "abc");
#ifdef DEBUG
        input = fopen(argv[1], "rb");
        output = fopen(argv[2], "rb");
        print_files(input, output);
#endif
    return 0;
}