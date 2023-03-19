#ifndef FLAG_H_INCLUDED
#define FLAG_H_INCLUDED

int bit(char resource, int which);
void addFlag(FILE *output, int compression, bool encrypt, char mask);
void checkFlag(FILE *output);
void printBits( unsigned int n );



#endif