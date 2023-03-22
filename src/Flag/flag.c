#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void printBits2( unsigned int n, int b )
{
    const int Bits = b;
    char tmp[ Bits + 1 ];

    for( int i = 0; i < Bits; ++i )
    {
        tmp[ Bits - i - 1 ] = '0' + n % 2;
        n /= 2;
    }

    tmp[ Bits ] = 0;
    printf("%s",tmp);
}


void addFlag(FILE *output, int compression, bool encrypt, char mask, char cntr) {
    unsigned char Flag = 0;

    switch(compression) {
        case 0:
            Flag <<= 2;
            Flag += 0b00;
            break;
        case 1:
            Flag <<= 2;
            Flag += 0b01;
            break;
        case 2:
            Flag <<= 2;
            Flag += 0b10;
            break;
        case 3:
            Flag <<= 2;
            Flag += 0b11;
            break;
        default:
            fprintf(stderr, "Wrong compression parametr!\n");
            break;
    }

    switch((int)encrypt) {
        case true:
            Flag <<= 1;
            Flag += 0b1;
            break;
        case false:
            Flag <<= 1;
            Flag += 0b0;
            break;
        default:
            fprintf(stderr, "Wrong encryption parametr!\n");
    }

    Flag <<= 1;
    Flag += 0b0; //jedno dodatkowe miejsce na informacje

    Flag <<= 4;
    Flag += mask;

    char *signature = "BJ";
    fwrite(&*signature, sizeof(char), 2, output);
    fwrite(&cntr, sizeof(char), 1, output);
    fwrite(&Flag, sizeof(unsigned char), 1, output);
}

void checkFlag(FILE *output) {
      char maskSzyfr = 0b00100000;
      char maskMask =  0b00001111;
      char maskComp =  0b11000000;
    unsigned char Flag;
    int check = fseek(output, 3, SEEK_SET);
    if(check != 0 ) {
        fprintf(stderr, "Error with fseek\n");
        return;
    }
    fread(&Flag, sizeof(char), 1, output);
    if(Flag & maskSzyfr) {
        printf("1. Encypting: true\n");
    } else {
        printf("1. Encypting: false\n");
    }
    unsigned char tmp = Flag;
    tmp = tmp & maskComp;
    tmp >>= 6;
    printf("2. Compression level: %d\n", tmp);

    tmp = Flag;
    tmp = tmp & maskMask;
    printf("3. Mask: ");
    printBits2((int)tmp,4);
    printf("\n");
}