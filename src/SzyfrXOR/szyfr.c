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
    srand(time(NULL));
    if(Verbose == true) {
        printf("==DEBUG==\n");
        printf("==DEBUG== XOR CIPHER\n");
    }
    unsigned short password;


    for(int i=0;i<strlen(pass);i++) {
//        printf(">%c -> ", pass[i]);
//        dec_to_binary((int)pass[i]);
//        printf("\n");
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

    //fprintf(pass, "%d", password);
    if(Verbose == true) {
        printf("==DEBUG==   password: [%d: ", password);
        dec_to_binary(password);
        printf("]\n");
        printf("==DEBUG==\n");
    }

    unsigned char x;

    while(fread(&x, sizeof(char), 1, input) == 1) {
        if(Verbose == true) {
            fprintf(stdout, "==DEBUG== in: %d -> '%c' [", x, x);
            dec_to_binary(x);
            printf("]\n");
        }

        x = x^password;

       if(Verbose == true) {
           fprintf(stdout, "==DEBUG== ot: %d -> '%c' [", x, x);
           dec_to_binary(x);
           printf("]\n");
       }
        fwrite(&x, sizeof(char), 1, output);
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
