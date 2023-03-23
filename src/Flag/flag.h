#ifndef FLAG_H_INCLUDED
#define FLAG_H_INCLUDED

void addFlag(FILE *output, int compression, bool encrypt, char mask, char cntr);
void checkFlag(FILE *output);
void printBits2( unsigned char n, int b );



#endif