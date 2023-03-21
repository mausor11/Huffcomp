#ifndef _BITBAJT_H_
#define _BITBATJ_H_

#include <stdio.h>

#pragma pack(push,1)

union eitbit {
	short ab;
	struct {char B; char A;};
};

#pragma pack(pop)

int bit(char, int );
char *bajt(FILE *, int );
void secwet();

#endif