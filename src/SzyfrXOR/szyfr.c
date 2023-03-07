#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "szyfr.h"

void XOR(FILE *input, FILE *output, int char_number, bool Verbose)
{
    if(Verbose == true) {
        printf("==DEBUG==\n");
        printf("==DEBUG== XOR CIPHER\n");
    }
    int password[char_number];
    FILE *pass = fopen("password", "w");
    for (int i = 0; i < char_number; i++) {
        password[i] = rand() % 2;
        fprintf(pass, "%d", password[i]);
    }

    if(Verbose == true) {
//#ifdef DEBUG
        printf("==DEBUG==   password: [");
    for(int i=0;i<char_number;i++)
        printf("%d", password[i]);
    printf("]\n");
//#endif

    }

    char x;
    int position = 0;

    while( (x = fgetc(input)) != EOF) {
        putc(algorithm(password[position], x - '0') + '0', output);
        if(Verbose == true) {
//#ifdef DEBUG
            printf("==DEBUG==     x:%d ? pass:%d = alg:%d\n", x - '0', password[position], algorithm(password[position], x - '0'));
//#endif
        }

        if(position == char_number -1)
            position = 0;
        else position++;
    }
    fclose(input);
    fclose(output);
}

int algorithm (int A, int B)
{
    if(A == B) {
        return 0;
    } else {
        return 1;
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
