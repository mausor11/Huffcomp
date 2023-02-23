#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "szyfr.h"



int main (int argc, char **argv) {

    srand(time(NULL));
    FILE *input = argc > 1 ? fopen(argv[1], "r") : stdin;

    if(input == NULL) {
        printf("File is not open!\n");
        return -1;
    }
    
    // y = ax + b
    int a = zakresRand(1,50);
    int b = zakresRand(0,25);
#ifdef DEBUG
    a = 3;
    b = 28;
#endif
    FILE *pass = fopen("password", "w");
    fprintf(pass, "%d %d", a, b);

    char sign;
    int len = 0;
    while((fscanf(input,"%c", &sign)) == 1) {
#ifdef DEBUG
        printf("%c", sign);
#endif
        len++;
    }
    fclose(input);
    int *content = malloc(len * sizeof(int)); 
    
    int i=0;
    input = fopen(argv[1], "r");
   
    while((fscanf(input,"%c", &sign)) == 1) {
        content[i] = newSign(a,b,sign);
        i++;
    }

    fclose(input);
#ifdef DEBUG
    for(int i=0;i<len;i++) {
        printf("%c", content[i]);
    }
    printf("\n");
#endif
    

    input = fopen("input", "w");
    for(int i=0;i<len;i++) {
        fprintf(input,"%c",content[i]);
    }
    
    
    
    
}