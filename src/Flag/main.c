#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "flag.h"

int main(int argc, char ** argv)
{

    char mask = 0b1001;
    FILE *ot = fopen("output", "wb");
    char cntr = 23;
    addFlag(ot, 1,false,mask, &cntr);
    fclose(ot);
    FILE *in = fopen("output", "r");
    checkFlag(in);
//    fclose(ot);
//    fclose(in);
    FILE *t = fopen("output", "r");
    char y;
    while((fscanf(t, "%c", &y)) == 1) {
        printBits2((int)y,8);
        printf("(%d)", y);
        printf("|");
    }
    printf("\n");
    return 0;

}