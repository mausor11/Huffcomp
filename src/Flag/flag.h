#ifndef FLAG_H_INCLUDED
#define FLAG_H_INCLUDED

void addFlag(FILE *output, int compression, bool encrypt, char mask, short cntr);
void checkFlag(FILE *output, char *flag, short *);
void printBits2( unsigned char n, int b );



#endif