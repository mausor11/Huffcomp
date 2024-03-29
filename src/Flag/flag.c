#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void printBits2( unsigned char n, int b )
{
    const int Bits = b;
    char tmp[ Bits + 1 ];

    for( int i = 0; i < Bits; ++i )
    {
        tmp[ Bits - i - 1 ] = '0' + n % 2;
        n /= 2;
    }

    tmp[ Bits ] = 0;
    for(int i=0;i< Bits; i++) {
        printf("%c", tmp[i]);
    }

}

void addFlag(FILE *output, int compression, bool encrypt, char mask, short cntr, unsigned char magicNumber, char isLast) {
	unsigned char Flag = 0;
	char *signature = malloc(2*sizeof(char));
	signature[0] = 'B';
	signature[1] = 'J';


	switch(compression) {
		case 0:
			Flag <<= 2;
			Flag += 0b00;
			break;
		case 1:
			Flag <<= 2;
			Flag += 0b01;
			break;
		case 2:
			Flag <<= 2;
			Flag += 0b10;
			break;
		case 3:
			Flag <<= 2;
			Flag += 0b11;
			break;
		default:
			fprintf(stderr, "Wrong compression parametr!\n");
			break;
	}

	switch((int)encrypt) {
		case true:
			Flag <<= 1;
			Flag += 0b1;
			break;
		case false:
			Flag <<= 1;
			Flag += 0b0;
			break;
		default:
			fprintf(stderr, "Wrong encryption parametr!\n");
	}

	/*	przypadek 12-bit, kiedy możemy mieć zero, jeden lub dwa
		'niepełne' (do 3) bajty - wykorzystano drugi znak inicjałów
		do przechowania informacji, czy są dwa 'niepełne' bajty;
		inicjały niezmienione, gdy jest jeden lub zero 'niepełnych'.

		Wyjaśnienie 'niepełnych' bajtów:
		załóżmy, że kompresujemy 12-bit kompresją plik o rozmiarze
		95B. Jako że dwa znaki 12-bitowej kompresji to 24 bity,
		czyli 3 * sizeof(char), to plik ma 2 'niepełne' bajty:
		95 % 3 = 2
	*/
	if(isLast == 2) {
		(*(signature+1))++;
		isLast--;
	}

    Flag <<= 1;
    Flag += isLast;
    /*	jedno dodatkowe miejsce na informacje
    	w przypadku 16-bit kompresji, tutaj przetrzymywana
    	jest informacja czy oryginalny plik miał
    	parzystą (0) czy nieparzystą (1) liczbę bajtów

		w przypadku 12-bit kompresji, (0) oznacza zero 'niepełnych'
		bajtów, a (1) oznacza albo 1 albo 2 'niepełne' bajty
	*/
    Flag <<= 4;
    Flag += mask;

    fwrite(signature, sizeof(char), 2, output);
    fwrite(&magicNumber, sizeof(char), 1, output);
    fwrite(&Flag, sizeof(unsigned char), 1, output);
    fwrite(&cntr, sizeof(short), 1, output);
	free(signature);
}

void checkFlag(FILE *output, char *sum, char *flag, short *lisc, bool Verbose) {
	unsigned short Liscie = 0;
	unsigned char Flag = 0;
	unsigned char Sum = 0;
	unsigned char tmp = 0;
	char maskLast = 0b00010000;
    char maskSzyfr = 0b00100000;
    char maskMask =  0b00001111;
    char maskComp =  0b11000000;

	if(Verbose){
		printf(
			"====\n"
			"==== CHECKING FLAG\n"
		);
	}

    int check = fseek(output, 2, SEEK_SET);
    if(check != 0 ) {
        fprintf(stderr, "Error with fseek\n");
        return;
    }
	fread(&Sum, sizeof(char), 1, output);

	*sum = Sum;
	fread(&Flag, sizeof(char), 1, output);
	*flag = Flag;
	if(Verbose) {
		printf("==== Flag: ");
		printBits2(Flag, 8);
		printf("\n");
		if(Flag & maskSzyfr) {
			printf("==== Encypting: true\n");
		} else {
			printf("==== Encypting: false\n");
		}
		tmp = Flag;
	    tmp = tmp & maskComp;
	    tmp >>= 6;
		printf("==== Compression level: %d\n", tmp);

		tmp = Flag;
		tmp = tmp & maskLast;
		printf("==== Origin file byte count: ");
		if(tmp)
			printf("odd\n");
		else
			printf("even\n");

	    tmp = Flag;
	    tmp = tmp & maskMask;
		printf("==== Mask: %d (", tmp);
		printBits2(tmp,4);
		printf(")\n");
	}
    check = fseek(output, 4, SEEK_SET);
    fread(&Liscie, sizeof(short), 1, output);
    *lisc = Liscie;
    if(Verbose){
		printf("==== Tree leaves: %d (", Liscie);
		printBits2(Liscie, 16);
		printf(")\n");
	}
}



void checkFlagfromFile(FILE *output) {
    unsigned char Flag = 0;
    unsigned short Liscie = 0;
    char maskSzyfr = 0b00100000;
    char maskMask =  0b00001111;
    char maskComp =  0b11000000;

    int check = fseek(output, 2, SEEK_SET);
    if(check != 0 ) {
        fprintf(stderr, "Error with fseek.\n");
        return;
    }
    check = fseek(output, 3, SEEK_SET);
    fread(&Flag, sizeof(char), 1, output);
	printf("Flag: ");
	printBits2(Flag, 8);
	printf(".\n");
	if(Flag & maskSzyfr) {
		printf("1. Encypting: true.\n");
	} else {
		printf("1. Encypting: false.\n");
	}
    unsigned char tmp = Flag;
    tmp = tmp & maskComp;
     tmp >>= 6;
	printf("2. Compression level: %d.\n", tmp);

    tmp = Flag;
    tmp = tmp & maskMask;
	printf("3. Mask: %d (", tmp);
	printBits2(tmp,4);
	printf(").\n");
    check = fseek(output, 4, SEEK_SET);
    fread(&Liscie, sizeof(short), 1, output);
	printf("4. Leaves: %d (", Liscie);
	printBits2(Liscie, 16);
	printf(").\n");
    check = fseek(output, 2, SEEK_SET);
    fread(&tmp, sizeof(char), 1, output);
    printf("5. MagicNum: %d (", tmp);
    printBits2(tmp, 8);
    printf(").\n");
}