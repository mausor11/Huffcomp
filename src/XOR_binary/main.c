#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void dec_to_binary (int liczba) {
    int i=31;
    bool ok=false;
    while(i--) {
        //warunek, który pozwoli ominąć początkowe zera
        if (liczba >> i & 1 & !ok)
            ok = true;
        if(ok) {
            printf("%d", (liczba>>i)&1);
        }
    }
}

int main(int argc, char ** argv)
{
    char *p;
    p = argv[1];
    printf("%d\n", strlen(p));
    return 0;
}
