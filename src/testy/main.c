#include <stdio.h>
#include <stdlib.h>

typedef struct L {
    char c;
    struct L *next;
}*lista_t, slist;

lista_t add(lista_t list, char c) {
    if(list == NULL) {
        list = malloc(sizeof(slist));
        list->c = c;
        list->next = NULL;
        return list;
    } else {
        lista_t tmp = list;
        while(tmp->next != NULL) tmp = tmp->next;
        tmp->next = malloc(sizeof(slist));
        tmp->next->next = NULL;
        tmp->next->c = c;
        return tmp;
    }
}

int bit(char resource, int which){	//which od 0 do 7
    int mask = 1 << which;
    return (resource & mask) ? 1 : 0;
}

void printBits( unsigned int n, int b )
{
    const int Bits = b;
    char tmp[ Bits + 1 ];

    for( int i = 0; i < Bits; ++i )
    {
        tmp[ Bits - i - 1 ] = '0' + n % 2;
        n /= 2;
    }

    tmp[ Bits ] = 0;
    printf("%s",tmp);
}

int main(int argc, char ** argv)
{
    char x = 0b



    return 0;


}