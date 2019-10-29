
#include <stdio.h>

void stampaCodifica(void *p, int size, int bigEndian);

void buildFloat(int size, int bigEndian, int *bit, void *p, int k);

void buildDouble(int size, int bigEndian, int *bit, void *p, int k);

void buildLongDouble(int size, int bigEndian, int *bit, void *p, int k);

void toBinary(unsigned char *f, int *bit, int j, int l);

void stampaBit(int *v, int n, int size);

int isBigEndian();

int main() {
    float af;
    double ad;
    long double ald;
    int bigEndian = 0;


    bigEndian = isBigEndian();

    printf("Inserisci un numero decimale\n");
    scanf("%Lf",&ald);
    //todo aggiungere scnaf per long double e cambiare compilatore
    ad = (double) ald;
    af = (float) ald;

    stampaCodifica((void *) &af, sizeof(af), bigEndian);
    stampaCodifica((void *) &ad, sizeof(ad), bigEndian);
    stampaCodifica((void *) &ald, sizeof(ald), bigEndian);

    return 0;
}

int isBigEndian(){
    unsigned int i = 1;
    char *c = (char *) &i;
    if (*c)
        return 0;
    else
        return 1;
}

void stampaCodifica(void *p, int size, int bigEndian) {

    int bit[size * 8];
    int i, k = 1;

    for (i = 0; i < size * 8; i++)
        bit[i] = 0;

    if (size == 4) {
        buildFloat(size,bigEndian,bit,p,k);
    } else if (size == 8) {
        buildDouble(size,bigEndian,bit,p,k);
    } else if (size == 16) {
        printf("\n***LONG DOUBLE***\n");
        buildLongDouble(size,bigEndian,bit,p,k);
    }
    stampaBit(bit, size * 8, size);
}

void buildFloat(int size, int bigEndian, int *bit, void *p, int k){
    unsigned char theFloat[4];

    printf("***FLOAT***\n");
    for (int i = 0; i < size; i++)
        *(theFloat + i) = *((unsigned char *) (p + i));
    if (bigEndian) {
        for (int j = 0; j < size - 1; j++) {
            toBinary(theFloat, bit, j, 0);
        }
    } else {
        for (int j = size - 1; j >= 0; j--) {
            toBinary(theFloat, bit, j, 8 * k - 1);
            k++;
        }
    }
}

void buildDouble(int size, int bigEndian, int *bit, void *p, int k){
    unsigned char theDouble[8];

    printf("\n***DOUBLE***\n");
    for (int i = 0; i < size; i++)
        *(theDouble + i) = *((unsigned char *) (p + i));
    if (bigEndian) {
        for (int j = 0; j < size - 1; j++) {
            toBinary(theDouble, bit, j, 0);
        }
    } else {
        for (int j = size - 1; j >= 0; j--) {
            toBinary(theDouble, bit, j, 8 * k - 1);
            k++;
        }
    }
}

void buildLongDouble(int size, int bigEndian, int *bit, void *p, int k){
    unsigned char theLongDouble[16];

    for (int i = 0; i < size; i++)
        *(theLongDouble + i) = *((unsigned char *) (p + i));
    if (bigEndian) {
        for (int j = 0; j < size - 1; j++) {
            toBinary(theLongDouble, bit, j, 0);
        }
    } else {
        for (int j = size - 1; j >= 0; j--) {
            toBinary(theLongDouble, bit, j, 8 * k - 1);
            k++;
        }
    }
}


void stampaBit(int *v, int n, int size) {
    int i;
    if (size == 4) {
        for (i = 0; i < n; i++, v++) {
            if (i == 0)
                printf("Bit segno: ");
            if (i == 1)
                printf("\nBit esponente: ");
            if (i == 9)
                printf("\nBit mantissa: ");
            printf("%d", *v);
        }
    } else if (size == 8) {
        for (i = 0; i < n; i++, v++) {
            if (i == 0)
                printf("Bit segno: ");
            if (i == 1)
                printf("\nBit esponente: ");
            if (i == 12)
                printf("\nBit mantissa: ");
            printf("%d", *v);
        }
    } else if (size == 16) {
        for (i = 0; i < n; i++, v++) {
            if (i == 0)
                printf("Bit segno: ");
            if (i == 1)
                printf("\nBit esponente: ");
            if (i == 16)
                printf("\nBit mantissa: ");
            printf("%d", *v);
        }
    }
}

void toBinary(unsigned char *f, int *bit, int j, int l) {
    int i = 0;
    do {
        bit[l - i] = f[j] % 2;
        f[j] = f[j] / 2;
        if (l == 0)
            i--;
        else
            i++;
    } while (f[j] > 0);
}