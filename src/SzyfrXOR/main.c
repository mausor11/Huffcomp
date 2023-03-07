#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "szyfr.h"

int main(int argc, char ** argv)
{
    srand(time(NULL));
    FILE *input = argc > 1 ? fopen(argv[1], "r") : stdin;
    FILE *output = argc > 2 ? fopen (argv[2], "w") : stdin;

    if(input == NULL) {
        printf("ERROR1\n");
        return -1;
    }
    int num = 12;   //ile bitów
    XOR(input, output, num, true);
#ifdef DEBUG
        input = fopen(argv[1], "r");
        output = fopen(argv[2], "r");
        print_files(input, output);
#endif
    return 0;
}