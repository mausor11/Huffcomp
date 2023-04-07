#ifndef FLAG_H_INCLUDED
#define FLAG_H_INCLUDED

void addFlag(FILE *output, int compression, bool encrypt, char mask, short cntr, unsigned char magicNumber, char isLast);
void checkFlag(FILE *output, char *, char *flag, short *, bool Verbose);
void checkFlagfromFile(FILE *output);
void printBits2( unsigned char n, int b );



#endif