#include <stdio.h>
#include <stdlib.h>

typedef enum{ZAFFIRO=1, SMERALDO=2, RUBINO=3, TOPAZIO=4}gemsNames;
typedef struct{
    int pos;
    int *current;
    int *max;
    int maxLenPsb;
    int actMaxLen;
}chain;

typedef struct{
    int mark[4];//vettore con le quantità di ogni gemma
    int name[4];//vettore con i 'nomi' di ogni gemma
    int nDist;
}gems;


int permRip(gems theGems, chain *theChain, int *bottomFlag);
int possibleGem(int sol, int prossimaGemma);
void printSol(int *arr, int len);

int main() {

    //int mark[4] = {0, 0, 0, 0};
    //int val[4] = {1, 2, 3, 4};
    //int maxDimAbs = 0;
    //int nDist;
    //int maxDim = 0;

    int bottomFlag = 0;
    chain theChain;
    gems theGems;

    theChain.pos = 0;
    theChain.actMaxLen = 0;
    theChain.maxLenPsb = 0;

    printf("Inserisci le quantita' di zaffiri, rubini, topazi e smeraldi:");
    scanf("%d %d %d %d", &theGems.mark[0], &theGems.mark[2], &theGems.mark[3], &theGems.mark[1]);

    theGems.nDist = 4;

    for(int i = 0; i < theGems.nDist; i++){
        theGems.name[i] = i+1; // inizializzo il vettore con i "nomi" delle gemme
        theChain.maxLenPsb += theGems.mark[i]; // calcolo numero di totale di gemme
    }
    theChain.max = (int *)malloc(theChain.maxLenPsb*sizeof(int));
    theChain.current = (int *)malloc(theChain.maxLenPsb*sizeof(int));

    permRip(theGems, &theChain, &bottomFlag); ////

    printSol( theChain.max, theChain.actMaxLen);
    free(theChain.current);
    free(theChain.max);
    return 0;
}


int permRip(gems theGems, chain *theChain, int *bottomFlag){
    //printf("\n> pos : %d", theChain->pos);
    //printf("\nmaxpsb:%d\n", theChain->maxLenPsb);
    int i, pGValue;
    if(theChain->pos >= theChain->maxLenPsb){
        *bottomFlag = 1;
        return 0;
    }

    for(i = 0; i < theGems.nDist && *bottomFlag == 0; i++){//per ogni livello di ricorsione scandisco tutti i valori possibili
        //printf("\n          controllo.%d", i);
        if(theChain->pos == 0){ // se sono al livello 0 di ricorsione non devo controllare di poter inserire o no una gemma
            pGValue = 1;
          //  printf("\n          scenta default : %d", pGValue);
        } else{
            pGValue = possibleGem(theChain->current[theChain->pos-1], theGems.name[i]);
        //printf("\n          verifica : %d", pGValue);
        //printf("\n          rimanenti: %d", theGems.mark[i]);
        }
        if(theGems.mark[i] > 0 && pGValue){
            theGems.mark[i]--;
            theChain->current[theChain->pos] = theGems.name[i];
            if(theChain->pos > theChain->actMaxLen){
                //printf("\n              0.maxdim:%d", *maxDim);
                theChain->actMaxLen = theChain->pos;
                //printf("\n              1.maxdim:%d", *maxDim);
                //printf("\n              solMax:");
                for(int j = 0; j <= theChain->pos; j++ ){
                    theChain->max[j] = theChain->current[j];
                    //printf("%d", theChain->max[j]);
                }
            }
            (theChain->pos)++;
            permRip(theGems, theChain, bottomFlag);
            (theChain->pos)--;
            theGems.mark[i]++;
        }
    }
    return 0;
}

int possibleGem(int sol1, int prossimaGemma){
    switch(prossimaGemma){
        case ZAFFIRO  : return (sol1 == TOPAZIO || sol1 == ZAFFIRO);
        case SMERALDO : return (sol1 == RUBINO  || sol1 == SMERALDO);
        case RUBINO   : return (sol1 == ZAFFIRO || sol1 == TOPAZIO);
        case TOPAZIO  : return (sol1 == RUBINO  || sol1 == SMERALDO);
        default : return 0;
    }
}

void printSol(int *arr, int len){

    printf("\ncollana massima di %d gemme : ", len+1);
    for(int j = 0; j <=len; j++){
        switch(arr[j]){
            case ZAFFIRO : printf(" Z "); break;
            case SMERALDO : printf(" S "); break;
            case RUBINO : printf(" R "); break;
            case TOPAZIO : printf(" T "); break;
            default: break;
        }
    }
    printf("\n");
}