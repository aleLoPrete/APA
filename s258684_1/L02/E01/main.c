#include <stdio.h>
#include <string.h>

enum maxnum{
    maxrowncol = 51,
};
int findB();
int findH();

typedef struct rettangolo{
    int x;
    int y;
    int altezza;
    int base;
}rettangolo;

int main() {
    FILE *pntSource;
    const char *PATHSource = "C:\\Users\\Alessandro\\Documents\\POLI\\APA\\lab\\svolgimenti\\lab02\\es01\\cmake-build-debug\\source.txt";
    int matrix[maxrowncol][maxrowncol];
    int rows, columns, i=0, j, area, base = 0, heigth = 0, heigthMax=0, baseMax=0, areaMax=0;
    struct rettangolo retMaxB = {0,0,0,0};
    struct rettangolo retMaxA = {0,0,0,0};
    struct rettangolo retMaxH = {0,0,0,0};

    pntSource = fopen(PATHSource, "r");
    if(pntSource==NULL)
        return -1;

    //immagazzino la matrice del file in una matrice di stringhe
    fscanf(pntSource, "%d %d", &rows, &columns);

    printf("\nmappa da file:\n");
    for(i=0; i<rows; i++) {
        for(j=0; j<columns; j++) {
            fscanf(pntSource, "%d", &matrix[i][j]);
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    for(i=0; i<rows; i++) {
        for(j=0; j<columns; j++) {
            if(matrix[i][j]==1){
                base = findB(matrix,i,j,rows,columns);
                heigth = findH(matrix,i,j,rows,columns);
                area = base * heigth;

                for(int f=i;f<i+heigth;f++){
                    for(int k=j;k<j+base;k++){
                        matrix[f][k]=0;
                    }
                }

                if(base >= retMaxB.base ) {
                    retMaxB.x=j;
                    retMaxB.y=i;
                    retMaxB.base = base;
                    retMaxB.altezza = heigth;
                }
                if(heigth>= retMaxH.altezza) {
                    retMaxH.x=j;
                    retMaxH.y=i;
                    retMaxH.base = base;
                    retMaxH.altezza = heigth;
                }
                if(area>= ( retMaxA.altezza * retMaxA.base) ) {
                    retMaxA.x=j;
                    retMaxA.y=i;
                    retMaxA.base = base;
                    retMaxA.altezza = heigth;
                }

                // aumento j per uscire dal rettangolo
                j += base;
            }
        }

    }

    printf("\nMax Base: estremo superiore SX: (%d,%d) base=%d, altezza=%d, Area=%d\n", retMaxB.x, retMaxB.y, retMaxB.base, retMaxB.altezza, (retMaxB.base * retMaxB.altezza) );
    printf("Max Area: estremo superiore SX: (%d,%d) base=%d, altezza=%d, Area=%d\n", retMaxA.x, retMaxA.y, retMaxA.base, retMaxA.altezza, (retMaxA.base * retMaxA.altezza) );
    printf("Max Altezza: estremo superiore SX: (%d,%d) base=%d, altezza=%d, Area=%d\n", retMaxH.x, retMaxH.y, retMaxH.base, retMaxH.altezza, (retMaxH.base * retMaxH.altezza) );

    return 0;
}

int findB(int matrix[maxrowncol][maxrowncol], int i, int j, int rows, int cols){
    //parto dalla casella e controllo la riga finché non trovo una casella bianca
    int base=0;
    for(int k=j; k<cols && matrix[i][k] == 1; k++ ) {
        base ++;
    }
    return base;
}

int findH(int matrix[maxrowncol][maxrowncol], int i, int j, int rows, int cols){
    //parto dalla casella e controllo la riga finché non trovo una casella bianca
    int heigth=0;
    for(int k=i; k<rows && matrix[k][j] == 1; k++ ) {
        heigth ++;
    }
    return heigth;
}