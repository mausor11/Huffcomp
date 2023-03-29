#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "szyfr.h"

int main(int argc, char ** argv)
{
    srand(time(NULL));
    FILE *input = argc > 1 ? fopen(argv[1], "rb+") : stdin;
    FILE *output = argc > 2 ? fopen (argv[2], "wb") : stdin;

    if(input == NULL) {
        printf("ERROR1\n");
        return -1;
    }
    char *password = "abc";
    fseek(input, 2, SEEK_SET);
    XOR2(input, 8, false, password);


    fclose(output);
    return 0;
}