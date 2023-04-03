#ifndef SZYFR_H_INCLUDED
#define SZYFR_H_INCLUDED

void XOR(FILE *input, FILE *output, int char_number, bool Verbose, char* password);
void XOR2(FILE *output, int char_number, bool Verbose, char* password);
void print_files(FILE *input, FILE *output);
void dec_to_binary (int num);
char MagicNum(FILE *input, unsigned char);

#endif