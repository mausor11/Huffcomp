#include "szyfr.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int zakresRand(int minimum, int maksimum) {
    return rand() % (maksimum - minimum + 1) + minimum;
}

int newSign(int a, int b, char sign) {
    if(sign == 127)
        return sign;
    
    int tmp = a*sign + b;
    
    if(tmp>=0 && tmp<=255) {
        return tmp;
    } else {
        int t = (tmp-255)%(255+1);
            t+= -1;
          return t;
    }

    
}