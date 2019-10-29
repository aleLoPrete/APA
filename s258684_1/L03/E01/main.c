#include <stdio.h>
#include <string.h>

int findB();
int findH();
void leggiMatrice(); // acquisisce la matrice da file e ritorna le dimensioni effettive
int riconosciRegione();
enum maxnum{
    maxrowncol = 51,
};

typedef struct {
    int x;
    int y;
    int altezza;
    int base;
}rettangolo;

int main() {
    FILE *pntSource;
    const char *PATHSource = "C:\\Users\\Alessandro\\Documents\\POLI\\APA\\lab\\svolgimenti\\lab03\\es01\\cmake-build-debug\\source.txt";
    int matrix[maxrowncol][maxrowncol];
    int rows=0, columns=0, i=0, j, area, base = 0, heigth = 0;
    rettangolo retMaxB = {0,0,0,0};
    rettangolo retMaxA = {0,0,0,0};
    rettangolo retMaxH = {0,0,0,0};
    int *pntRows = &rows, *pntCols = &columns;
    int *pntBase = &base, *pntHeigth = &heigth;

    pntSource = fopen(PATHSource, "r");
    if(pntSource==NULL)
        return -1;

    leggiMatrice(pntSource, matrix, maxrowncol, pntRows, pntCols );

    for(i=0; i<rows; i++) {
        for(j=0; j<columns; j++) {
            if(riconosciRegione(matrix,rows,columns,i,j,&base,&heigth)){
                printf("il rettangolo con estremo in %d %d ha base=%d e altezza=%d\n",j,i,base,heigth);
                area = base*heigth;
                if(base > retMaxB.base ) {
                    retMaxB.x=j;
                    retMaxB.y=i;
                    retMaxB.base = base;
                    retMaxB.altezza = heigth;
                }
                if(heigth> retMaxH.altezza) {
                    retMaxH.x=j;
                    retMaxH.y=i;
                    retMaxH.base = base;
                    retMaxH.altezza = heigth;
                }
                if(area> ( retMaxA.altezza * retMaxA.base) ) {
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
    printf("matrice dopo l'esame:\n");
    for(i=0;i<rows;i++){
        for(j=0;j<columns;j++){
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    printf("\nMax Base: estremo superiore SX: (%d,%d) base=%d, altezza=%d, Area=%d\n", retMaxB.x, retMaxB.y, retMaxB.base, retMaxB.altezza, (retMaxB.base * retMaxB.altezza) );
    printf("Max Area: estremo superiore SX: (%d,%d) base=%d, altezza=%d, Area=%d\n", retMaxA.x, retMaxA.y, retMaxA.base, retMaxA.altezza, (retMaxA.base * retMaxA.altezza) );
    printf("Max Altezza: estremo superiore SX: (%d,%d) base=%d, altezza=%d, Area=%d\n", retMaxH.x, retMaxH.y, retMaxH.base, retMaxH.altezza, (retMaxH.base * retMaxH.altezza) );

    return 0;
}

void leggiMatrice(FILE *pnt, int theMatrice[maxrowncol][maxrowncol], int theMax, int *row, int *col){

    int i=0,j=0;
    int theRow, theCol;

    fscanf(pnt, "%d %d", &theRow, &theCol);
    *row += theRow;
    *col += theCol;

    printf("\nmappa da file:\n");
    for(i=0; i<theRow; i++) {
        for(j=0; j<theCol; j++) {
            fscanf(pnt, "%d", &theMatrice[i][j]);
            printf("%d ", theMatrice[i][j]);
        }
        printf("\n");
    }

    return;

}

int riconosciRegione(int theMatrix[maxrowncol][maxrowncol],int theRows, int theColumns,int i, int j, int *thePntBase, int *thePntHeigth){

    int theBase, theHeigth;

    if(theMatrix[i][j]==1) {
        theBase = findB(theMatrix,i,j,theColumns );
        theHeigth = findH(theMatrix,i,j,theRows);

        ////cancello tutto il rettangolo per evitare di riesaminare erroneamente la regione quando cambio riga

        for(int f=i;f<i+theHeigth;f++){
            for(int k=j;k<j+theBase;k++){
                theMatrix[f][k]=0;
            }
        }



        *thePntHeigth = theHeigth;
        *thePntBase = theBase;
        return 1;
    }

    return 0;
}

int findB(int theMatrix[maxrowncol][maxrowncol], int i, int j, int cols){
    //parto dalla casella e controllo la riga finché non trovo una casella bianca
    int base=0;
    for(int k=j; k<cols && theMatrix[i][k] == 1; k++ ) {
        base ++;
    }
    return base;
}

int findH(int theMatrix[maxrowncol][maxrowncol], int i, int j, int rows){
    //parto dalla casella e controllo la riga finché non trovo una casella bianca
    int heigth=0;
    for(int k=i; k<rows && theMatrix[k][j] == 1; k++ ) {
        heigth ++;
    }
    return heigth;
}