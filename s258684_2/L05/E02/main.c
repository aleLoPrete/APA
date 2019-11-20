#include <stdio.h>
#include <stdlib.h>

int **malloc2dR(int *nr, int *nc);
void separa(int **mat, int nr, int nc, int *blacks, int *whites);
void printArray(int *array, int lenArray);

int main() {
    int **matrix, nR, nC, i, *blacks, *whites;

    matrix = malloc2dR(&nR, &nC);

    blacks = malloc((nR * nC / 2 + 1) * sizeof(int));
    whites = malloc((nR * nC) / 2 * sizeof(int));

    separa(matrix, nR, nC, blacks, whites);

    printArray(blacks, (nR * nC / 2 + 1) );
    printf("\n");
    printArray(whites,(nR * nC / 2));

    free(whites);
    free(blacks);
}

int **malloc2dR(int *nR, int *nC) {
    FILE *fp;
    int **matrix, i, j;

    fp = fopen("matrice.txt", "r");
    if(fp == NULL)
        return -1;

    fscanf(fp, "%d %d", nR, nC);

    matrix = malloc(*nR * sizeof(int *));

    for (i = 0; i < *nR; i++)
        matrix[i] = malloc(*nC * sizeof(int));

    for (i = 0; i < *nR; i++) {
        for (j = 0; j < *nC; j++)
            fscanf(fp, "%d", &matrix[i][j]);
    }

    return matrix;
}

void separa(int **mat, int nr, int nc, int *blacks, int *whites) {
    int i, j, k = 0, m = 0, n = 0;

    for (i = 0; i < nr; i++) {
        for (j = 0; j < nc; j++) {
            if (k % 2 == 0) {
                blacks[m] = mat[i][j];
                m++;
            } else {
                whites[n] = mat[i][j];
                n++;
            }
            k++;
        }
    }
}

void printArray(int *array, int lenArr){
    for (int i = 0; i < lenArr; i++)
        printf("%d ", array[i]);
}