#include <stdio.h>
#include <ctype.h>
#define N 8
#define M 7

void acqMatrix( int *fp, int matrice[N][M] ); // acquisisce una matrice scritta in un file passato per parametro
int printCapoliste(int matrice[N][M] ); // stampa tutte le capoliste di tutte le giornate, (prende come parametro matrice contenente i punteggi delle squadre(per riga) per ogni giornata(le colonne))
void printCapolistaGiornata( int vettore[N] ); // stampa l'indice delle capoliste della giornata (vettore dei punteggi) passata come parametro

int main() {
    int i, j, champMatrix[N][M];
    FILE *fp;

    fp = fopen("campionato.txt", "r");

    // acquisisco la matrice dal file
    acqMatrix(fp,champMatrix);

    fclose(fp);

    // stampo le capoliste per ogni giornata
    printCapoliste(champMatrix);

    return 0;
}


void acqMatrix(int *fp, int matrice[N][M]) {

    int i, j;

    for( i=0; i<N; i++) {
        for ( j=0; j<M; j++) {
            fscanf((FILE *)fp, " %d ", &matrice[i][j] );
        }
    }

}

int printCapoliste(int matrice[N][M]) {

    int i, j;
    int pntSquadra[N] = {0};

    //inizializzo a zero i punteggi di tutte le squadre



    for ( j=0; j<M; j++ ) {
        for( i=0; i<N; i++ ) {
            pntSquadra[i] += matrice[i][j];
        }

        printf("le/la squadra/e per la giornata %d sono:", j);
        printCapolistaGiornata(pntSquadra);
        printf("\n");
    }
}

void printCapolistaGiornata(int vettore[N]) { //stampa l'indice della squadra con punteggio più alto, se ci sono pareggi stampa tutti i primi punteggi uguali
    int i, max=0;

    for(i=0; i<N; i++) {
        if(vettore[i]>max) {
            max=vettore[i];
        }
    }

    for(i=0; i<N; i++) {
        if(vettore[i]>=max) {
            printf(" %d ", i);
        }

    }

}