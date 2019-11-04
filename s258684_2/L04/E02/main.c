#include <stdio.h>
#define NUM 10

int majority(int *theArray, int N);
int computeOcc(int n, int *array, int dimArr);

int main() {
    int array[NUM]={1, 2, 1, 0, 1, 2, 1, 0, 1, 1};
    int magg;

    magg = majority(array,NUM);

    if(magg == -1) {
        printf("\nnon esiste un elemento maggioritario all'interno del vettore\n");
        return -1;
    }
    printf("l'elemento maggioritario e' %d", magg);
    return 0;
}


int majority(int *theArray, int N){
    int occNumSx = 0, occNumDx = 0, numSx, numDx, dimDx, dimSx;

    dimSx = N/2;

    dimDx = (int)(N % 2) != 0 ? (N / 2) + 1 : N/2; //devo gestire la dimensione del sub-array nel caso di elementi dispari nel vettore di partenza

    if(N==1)
        return theArray[0];

    numSx = majority(theArray,dimSx);
    numDx = majority((theArray+dimSx),dimDx);

    occNumSx = computeOcc(numSx,theArray,N);
    occNumDx = computeOcc(numDx,theArray,N);

    if(occNumSx>=occNumDx && occNumSx>N/2)
        return numSx;
    else if(occNumDx>N/2)
        return numDx;
    return -1;
}

int computeOcc(int n, int *array, int dimArr) {
    int numOcc=0;
    for(int i = 0; i<dimArr; i++){
        if(array[i]==n)
            numOcc++;
    }
    return numOcc;
}

