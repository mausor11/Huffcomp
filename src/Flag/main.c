#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "flag.h"

int main(int argc, char ** argv)
{

    char mask = 0b0110;
    FILE *ot = fopen("output", "wb");
    addFlag(ot, 1,true,mask);

    return 0;

}