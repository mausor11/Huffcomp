#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "flag.h"

int main(int argc, char ** argv)
{

    char mask = 0b1111;
    FILE *ot = fopen("output", "wb");
    addFlag(ot, 2,true,mask);
    fclose(ot);
    FILE *in = fopen("output", "r");
    checkFlag(in);
    return 0;

}