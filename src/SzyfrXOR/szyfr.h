#ifndef SZYFR_H_INCLUDED
#define SZYFR_H_INCLUDED

void XOR(FILE *input, FILE *output, int char_number, bool Verbose, char* password);
int algorithm (int A, int B);
void print_files(FILE *input, FILE *output);
void dec_to_binary (int num);
char MagicNum(FILE *input, unsigned char);

#endif