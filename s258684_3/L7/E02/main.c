#include <stdio.h>
#include <stdlib.h>

typedef enum{ZAFFIRO=1, SMERALDO=2, RUBINO=3, TOPAZIO=4}gemsNames;
typedef struct{
    int pos;
    int *current;
    int *max;
    int maxLenPsb;
    int actMaxLen;
    int actMaxValue;
    int maxValuePsb;
    int maxRep;
}chain;

typedef struct{
    int mark[4];//vettore con le quantità di ogni gemma
    int name[4];//vettore con i 'nomi' di ogni gemma
    int val[4];
    int nDist;
}gems;


int permRip(gems *theGems, chain *theChain, int *bottomFlag);
int possibleGem(int sol, int prossimaGemma);
void printSol(chain *theChain);
int maxRep(chain *theChain, gems *theGems, int i);
int checkZS(int *arr, int len);
int valueCalc();

int main() {
    int bottomFlag = 0;
    chain theChain;
    gems theGems;

    theChain.pos = 0;
    theChain.actMaxLen = 0;
    theChain.actMaxValue = 0;
    theGems.nDist = 4;

    printf("Inserisci le quantita' di gemme(Z,R,T,S), i rispettivi valori e le ripetizion massime:");
    scanf("%d %d %d %d   %d %d %d %d    %d", &theGems.mark[0], &theGems.mark[2], &theGems.mark[3], &theGems.mark[1],
            &theGems.val[0], &theGems.val[2], &theGems.val[3], &theGems.val[1],                   &theChain.maxRep);


    for(int i = 0; i < theGems.nDist; i++){
        theGems.name[i] = i+1; // inizializzo il vettore con i "nomi" delle gemme
        theChain.maxLenPsb += theGems.mark[i];// calcolo numero di totale di gemme
        theChain.maxValuePsb += theGems.mark[i]*theGems.val[i];// calcolo il valore totale delle gemme
    }
    theChain.max = (int *)malloc(theChain.maxLenPsb*sizeof(int));
    theChain.current = (int *)malloc(theChain.maxLenPsb*sizeof(int));

    permRip(&theGems, &theChain, &bottomFlag); ////

    printSol(&theChain);
    free(theChain.current);
    free(theChain.max);
    return 0;
}


int permRip(gems *theGems, chain *theChain, int *bottomFlag){
    int i, pGValue, value;
    if(theChain->pos >= theChain->maxLenPsb){
        *bottomFlag = 1;
        return 0;
    }
    for(i = 0; i < theGems->nDist && *bottomFlag == 0; i++){//per ogni livello di ricorsione scandisco tutti i valori possibili delle gemme
        if(theChain->pos == 0){ // se sono al livello 0 di ricorsione non devo controllare di poter inserire o no una gemma
            pGValue = 1;
        } else{
            pGValue = possibleGem(theChain->current[theChain->pos-1], theGems->name[i]);
        }
        if(theGems->mark[i] > 0 && !maxRep(theChain, theGems, i) && pGValue){ // controllo se la gemma c'è ancora e se posso prenderla
            theGems->mark[i]--;
            theChain->current[theChain->pos] = theGems->name[i]; //inserzione della gemma esaminata
            value = valueCalc(theChain, theGems);
            if(theChain->pos >= theChain->actMaxLen && value >= theChain->actMaxValue && checkZS(theChain->current, theChain->pos)){ // se trovo una dimensione più grande di quella trovata fin'ora la salvo
                theChain->actMaxLen = theChain->pos;
                theChain->actMaxValue = value;
                for(int j = 0; j <= theChain->pos; j++ ){
                    theChain->max[j] = theChain->current[j];
                }
            }
            (theChain->pos)++;
            permRip(theGems, theChain, bottomFlag);
            (theChain->pos)--;
            theGems->mark[i]++;
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

void printSol(chain *theChain){

    if(theChain->actMaxLen > 0){
        printf("\ncollana massima di %d gemme : ", theChain->actMaxLen+1);
        for(int j = 0; j <=theChain->actMaxLen; j++){
            switch(theChain->max[j]){
                case ZAFFIRO : printf(" Z "); break;
                case SMERALDO : printf(" S "); break;
                case RUBINO : printf(" R "); break;
                case TOPAZIO : printf(" T "); break;
                default: break;
            }
        }
        printf("\n");
        printf("valore catena : %d", theChain->actMaxValue);
    }
    else printf("\nnon esiste nessuna sequenza accettabile per queste quantità di gemme");
}

int checkZS(int *array, int len) { //controlla se il numero di zaffiri è minore o uguale a quello degli smeraldi
    int countZ = 0, countS = 0;

    for(int i = 0; i <= len; i++){
        if(array[i] == ZAFFIRO) countZ++;
        else if(array[i] == SMERALDO) countS++;
    }
    if(countZ > countS) return 0;
    if(countZ <= countS) return 1;
}

int valueCalc(chain *theChain, gems *theGems){
    int value = 0;
    for(int i = 0; i <= theChain->pos ; i++){ //minore o uguale perché devo calcolare il valore della catena fino alla gemma che ho appena inserito
        switch(theChain->current[i]){
            case ZAFFIRO : value += theGems->val[0]; break;
            case SMERALDO : value += theGems->val[1]; break;
            case RUBINO : value += theGems->val[2]; break;
            case TOPAZIO : value += theGems->val[3]; break;
            default : break;
        }
    }
    return value;
}

int maxRep(chain *theChain, gems *theGems, int i){ // funzione che verifica se la gemma che voglio mettere ha già raggiunto le ripetizioni massime
    int counter = 0, seqFlag = 1;
    for(int j = (theChain->pos-1); j >= 0 && theChain->current[j] == theGems->name[i] ; j--){ //sto esaminando se mettere o no la gemma all'indice 'theChain->pos'
        counter++; // conto a ritroso quante gemme uguali sono immediatamente precedenti a quella che sto esaminando
    }
    if(counter >= theChain->maxRep) return 1; // ho raggiunto le ripetizioni massime
    return 0; // non ho raggiunto le ripetizioni massime
}