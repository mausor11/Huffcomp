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
    
    if(tmp>=33 && tmp<=126) {
        return tmp;
    } else {
        int t = (tmp-126)%(126-33+1);
        
        if(t == 0) {
            return 126;
        } else {
            t+= 32;
          return t;
        }
    }

    
}