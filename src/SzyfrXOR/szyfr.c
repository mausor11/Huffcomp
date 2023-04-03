#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "szyfr.h"

void dec_to_binary (int num) {
    int i=31;
    bool ok=false;
    while(i--) {
        if (num >> i & 1 & !ok)
            ok = true;
        if(ok) {
            printf("%d", (num>>i)&1);
        }
    }
}


void XOR(FILE *input, FILE *output, int char_number, bool Verbose, char *pass )
{
    unsigned short password = 0;

    for(int i=0;i<strlen(pass);i++) {
        password ^= pass[i];
    }
    switch (char_number) {
        case 8:
            password >> 8;
            break;
        case 12:
            password >> 4;
            break;
        case 16:
            break;
        default:
            fprintf(stderr, "Wrong char number!\n");
            break;
    }

    unsigned char x = 0;

    while(fread(&x, sizeof(char), 1, input) == 1) {
        x = x^password;
        fwrite(&x, sizeof(char), 1, output);
    }
}

void XOR2(FILE *output, int char_number, bool Verbose, char *pass )
{
    unsigned short password = 0;

    for(int i=0;i<strlen(pass);i++) {
        password ^= pass[i];
    }
    switch (char_number) {
        case 8:
            password >> 8;
            break;
        case 12:
            password >> 4;
            break;
        case 16:
            break;
        default:
            fprintf(stderr, "Wrong char number!\n");
            break;
    }

    unsigned char x = 0;

    while(fread(&x, sizeof(char), 1, output) == 1) {

        x = x^password;
        fseek(output, -1, SEEK_CUR);
        fwrite(&x, sizeof(char), 1, output);
    }
}


void print_files(FILE *input, FILE *output)
{
    char tmp;
    printf("input : [");
    while((tmp = fgetc(input)) != EOF)
        printf("%d", tmp - '0');
    printf("]\n");
    printf("output: [");
    while((tmp = fgetc(output)) != EOF)
        printf("%d", tmp - '0');
    printf("]\n");
}

char MagicNum(FILE *input, unsigned char magicNumber) {
    unsigned char x = 0;
    int check = fseek(input, 0, SEEK_SET);
    for(int i=0;i<2;i++) {
        if(fread(&x, sizeof(char), 1, input)) {
            magicNumber = x^magicNumber;
        } else {
            fprintf(stderr,"There was an error while reading the file. Aborting.\n");
            return EXIT_FAILURE;
        }
    }
    check = fseek(input, 1, SEEK_CUR);
    if(check != 0 ) {
            fprintf(stderr, "Error with fseek\n");
            return EXIT_FAILURE;
    }
    while((fread(&x, sizeof(char), 1, input)) == 1) {
        magicNumber = x^magicNumber;
    }
    return magicNumber;
}


