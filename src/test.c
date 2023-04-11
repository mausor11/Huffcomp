#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Flag//flag.h"

void compFiles(FILE *t1, FILE *t2, int i) {
    int errors = 0;
    int position = 0;
    char temp1 = fgetc(t1);
    char temp2 = fgetc(t2);

    while(temp1 != EOF && temp2 != EOF) {
        position++;
        if(temp1 != temp2) {
            errors++;
            printf("Comp type %d: Found error on position %d\n", i, position);
        }
        temp1 = fgetc(t1);
        temp2 = fgetc(t2);
    }
    printf("Comp type %d: Errors found: %d\n", i, errors);
}

void wrongPassword(FILE *t1, FILE *t2, int i) {
    char temp1 = fgetc(t1);
    char temp2 = fgetc(t2);

    while(temp1 != EOF && temp2 != EOF) {
        if(temp1 != temp2) {
            printf("Comp type %d: Detected wrong password\n", i);
            break;
        }
    }

}

int main() {
    int i= 1;
    FILE *t1 = fopen("PlikiTestowe/test1", "r");
    FILE *t2 = fopen("PlikiTestowe/test2", "r");
    FILE *t3 = fopen("PlikiTestowe/test3", "r");
    FILE *terminator = fopen("PlikiTestowe/Pan_Tadeusz", "r");

    t1 = fopen("PlikiTestowe/test0", "r");
    terminator = fopen("PlikiTestowe/Pan_Tadeusz", "r");
    printf("==== Comp type 0 TEST\n");
    if(t1) {
        i = 0;
        fseek(terminator, 0, SEEK_SET);
        compFiles(t1, terminator, i);
        fclose(t1);
    }

    t1 = fopen("PlikiTestowe/test1", "r");
    terminator = fopen("PlikiTestowe/Pan_Tadeusz", "r");

    printf("\n");
    printf("==== -o1 -o2 -o3 TEST\n");
    if(t1) {
        i = 1;
        fseek(terminator, 0 ,SEEK_SET);
        compFiles(t1, terminator, i);
        fclose(t1);
    }
    if(t2) {
        i = 2;
        fseek(terminator, 0 ,SEEK_SET);
        compFiles(t2, terminator, i);
        fclose(t2);
    }
    if(t3) {
        i = 3;
        fseek(terminator, 0 ,SEEK_SET);
        compFiles(t3, terminator, i);
        fclose(t3);
    }

    t1 = fopen("PlikiTestowe/pic1d.png", "r");
    t2 = fopen("PlikiTestowe/pic2d.png", "r");
    t3 = fopen("PlikiTestowe/pic3d.png", "r");
    terminator = fopen("PlikiTestowe/no_way.png", "r");

    printf("\n");
    printf("==== -o1 -o2 -o3 PICTURE TEST\n");
    if(t1) {
        i = 1;
        fseek(terminator, 0 ,SEEK_SET);
        compFiles(t1, terminator, i);
        fclose(t1);
    }
    if(t2) {
        i = 2;
        fseek(terminator, 0 ,SEEK_SET);
        compFiles(t2, terminator, i);
        fclose(t2);
    }
    if(t3) {
        i = 3;
        fseek(terminator, 0 ,SEEK_SET);
        compFiles(t3, terminator, i);
        fclose(t3);
    }

    t1 = fopen("PlikiTestowe/test1e", "r");
    t2 = fopen("PlikiTestowe/test2e", "r");
    t3 = fopen("PlikiTestowe/test3e", "r");
    terminator = fopen("PlikiTestowe/Pan_Tadeusz", "r");

    printf("\n");
    printf("==== ENCRYPTION TEST\n");
    if(t1) {
        i = 1;
        fseek(terminator, 0 ,SEEK_SET);
        compFiles(t1, terminator, i);
        fclose(t1);
    }
    if(t2) {
        i = 2;
        fseek(terminator, 0 ,SEEK_SET);
        compFiles(t2, terminator, i);
        fclose(t2);
    }
    if(t3) {
        i = 3;
        fseek(terminator, 0 ,SEEK_SET);
        compFiles(t3, terminator, i);
        fclose(t3);
    }

    t1 = fopen("PlikiTestowe/scenario1", "r");

    printf("\n");
    printf("==== SIDE SCENARIOS\n");
    printf("1.	No arguments = default settings\n");
    checkFlagfromFile(t1);
    printf("\n");
/*
    printf("2.	Different compression level and forced decompression level\n");
	
*/
    fclose(t1);
    fclose(terminator);
}
