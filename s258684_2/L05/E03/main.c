#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#define MAXN 31
#define MAXR 1000


typedef struct {
    char idCode[MAXN];
    char partenza[MAXN];
    char arrivo[MAXN];
    char dataLog[MAXN];
    char StartingTime[MAXN];
    char ArrivalTime[MAXN];
    int delay;
}log;



log *storeLog(FILE *thePnt);
void printMenu();
int dichotomicSearching(char theSearchKey[MAXN], log **theLogArray, int l, int r);
int dichotomicSearchingWrapper(char theSearchKey[MAXN], log **theLogArray, int len);
void printPointerLog(log **logArray, int start, int logLen, int cmd);
void sortPointerPerDate(log **theArrayPointerDate, int theLogLen );
void sortPointerPerTime(log **theArrayPointer, int theLogLen );
void sortPointerPerPartenza(log **theArrayPointer, int theLogLen );
void sortPointerPerArrivo(log **theArrayPointer, int theLogLen );
void sortPointerPerCode(log **theArrayPointer, int theLogLen );

int main() {
    FILE *pnt;
    const char PATHfile[30] = "corse.txt";
    log *logArray;
    log **arrayPointerDate;
    log **arrayPointerPartenza;
    log **arrayPointerCode;
    log **arrayPointerArrivo;
    int logLen, choice=0, cmd;
    char searchKey[MAXN];
    int matchingIndex;

    pnt = fopen(PATHfile, "r");
    if(pnt == NULL)
        return -1;

    logArray = storeLog(pnt);
    logLen = sizeof(logArray);
    printf(" size : %d", logLen);
    fclose(pnt);

    //alloco dinamicamente i vettori per gli ordinamenti
    arrayPointerCode = arrayPointerArrivo = arrayPointerPartenza = arrayPointerDate = (log **)malloc(logLen* sizeof(log*));

    //inizializzo tutti i puntatori di odinamento al log acquisito
    for(int i=0; i<logLen; i++){
        arrayPointerDate[i] = &logArray[i];
        arrayPointerPartenza[i] = &logArray[i];
        arrayPointerArrivo[i] = &logArray[i];
        arrayPointerCode[i] = &logArray[i];
    }
    sortPointerPerTime(arrayPointerDate,logLen); //time
    sortPointerPerDate(arrayPointerDate,logLen); //date
    sortPointerPerPartenza(arrayPointerPartenza,logLen); //partenza
    sortPointerPerArrivo(arrayPointerArrivo,logLen); //arrivo
    sortPointerPerCode(arrayPointerCode,logLen); //code

    while(1) {
        printMenu();

        printf("\nscelta : \n");
        scanf("%d", &choice);

        switch(choice) {
            case -1:
                return 0;
            case 1:
                printf("inserire 1 per stampare a video, 0 per stampare su file(log.txt)");
                scanf("%d", &cmd);
                printPointerLog(arrayPointerCode,0,logLen, cmd);
                break;
            case 2:
                printPointerLog(arrayPointerDate,0,logLen,1);
                break;
            case 3:
                printPointerLog(arrayPointerCode,0,logLen,1);
                break;

            case 4:
                printPointerLog(arrayPointerPartenza,0,logLen,1);
                break;
            case 5 :
                printPointerLog(arrayPointerArrivo,0,logLen,1);
                break;
            case 6 :
                printf("\ninserisci la stazione di partenza come ricerca\n");
                scanf("%s", searchKey);
                matchingIndex = dichotomicSearchingWrapper(searchKey, arrayPointerPartenza, logLen);

                if(matchingIndex == -1) {
                    printf("\nchiave non trovata\n");
                    break;
                }
                printf("la chiave e' stata trovata nella tratta con indice %d:\n", matchingIndex);
                printPointerLog(arrayPointerPartenza, matchingIndex, 1, 1);
                break;

        }
    }


}


void printMenu(){
    printf("\n****MENU****\n");
    printf("-1>termina programma\n");
    printf("1>stampa log (default : codice tratta)\n");
    printf("2>visualizza in ordine di data\n");
    printf("3>visualizza in ordine di codice tratta\n");
    printf("4>visualizza in ordine di stazione partenza\n");
    printf("5>visualizza in ordine di stazione arrivo\n");
    printf("6>ricerca una tratta per stazione di partenza\n");
}

log *storeLog(FILE *thePnt){
    log *theLogArray;
    int n=2, i=0;
    theLogArray = (log*)malloc(n*sizeof(log));

    while( fscanf (thePnt,"%s %s %s %s %s %s %d", theLogArray[i].idCode, theLogArray[i].partenza, theLogArray[i].arrivo,
                  theLogArray[i].dataLog, theLogArray[i].StartingTime, theLogArray[i].ArrivalTime, &(theLogArray[i].delay)) != EOF ){
        i++;
        if(i==n){
            n*=2;
            theLogArray = (log *)realloc(theLogArray,n*sizeof(log));
        }
    };
    return theLogArray;
}


void printPointerLog(log **logArray, int start, int logLen, int cmd){
    int i=start;
    int r = i+logLen;
    if(cmd == 1){
        for(i=start; i<r; i++) {
            printf("\n%s %s %s %s %s -> %s delay: %d minuti\n", (*logArray[i]).idCode, logArray[i]->partenza, logArray[i]->arrivo,
                   logArray[i]->dataLog,logArray[i]->StartingTime, logArray[i]->ArrivalTime, logArray[i]->delay);
        }
    }
    else {
        FILE *fp = fopen("log.txt", "w");
        for(i=start; i<r; i++) {
            fprintf(fp,"\n%s %s %s %s %s -> %s delay: %d minuti", (*logArray[i]).idCode, logArray[i]->partenza, logArray[i]->arrivo,
                    logArray[i]->dataLog,logArray[i]->StartingTime, logArray[i]->ArrivalTime, logArray[i]->delay);
        }
        fclose(fp);
    }

}

void sortPointerPerDate(log **theArrayPointer, int theLogLen ){
    log *temp;

    for(int i =0; i<theLogLen; i++) {
        for(int l=0; l<theLogLen-1; l++){
            if(strcmp(theArrayPointer[l]->dataLog,theArrayPointer[l+1]->dataLog)>0){
                temp = theArrayPointer[l+1];
                theArrayPointer[l+1]=theArrayPointer[l];
                theArrayPointer[l] = temp;
            }
        }
    }
}

void sortPointerPerTime(log **theArrayPointer, int theLogLen ){
    log *temp;

    for(int i =0; i<theLogLen; i++) {
        for(int l=0; l<theLogLen-1; l++){
            if(strcmp(theArrayPointer[l]->StartingTime,theArrayPointer[l+1]->StartingTime)>0){
                temp = theArrayPointer[l+1];
                theArrayPointer[l+1]=theArrayPointer[l];
                theArrayPointer[l] = temp;
            }
        }
    }
}


void sortPointerPerPartenza(log **theArrayPointer, int theLogLen ){
    log *temp;
    for(int i =0; i<theLogLen; i++) {
        for(int l=0; l<theLogLen-1; l++){
            if(strcmp(theArrayPointer[l]->partenza,theArrayPointer[l+1]->partenza)>0){
                temp = theArrayPointer[l+1];
                theArrayPointer[l+1]=theArrayPointer[l];
                theArrayPointer[l] = temp;
            }
        }
    }
}
void sortPointerPerArrivo(log **theArrayPointer, int theLogLen ){
    log *temp;
    for(int i =0; i<theLogLen; i++) {
        for(int l=0; l<theLogLen-1; l++){
            if(strcmp(theArrayPointer[l]->arrivo,theArrayPointer[l+1]->arrivo)>0){
                temp = theArrayPointer[l+1];
                theArrayPointer[l+1]=theArrayPointer[l];
                theArrayPointer[l] = temp;
            }
        }
    }
}
void sortPointerPerCode(log **theArrayPointer, int theLogLen ){
    log *temp;
    for(int i =0; i<theLogLen; i++) {
        for(int l=0; l<theLogLen-1; l++){
            if(strcmp(theArrayPointer[l]->idCode,theArrayPointer[l+1]->idCode)>0){
                temp = theArrayPointer[l+1];
                theArrayPointer[l+1]=theArrayPointer[l];
                theArrayPointer[l] = temp;
            }
        }
    }
}



int dichotomicSearchingWrapper(char theSearchKey[MAXN], log **theLogArray, int len){
    int l =0;
    int r = len-1;
    return dichotomicSearching(theSearchKey,theLogArray,l,r);
}

int dichotomicSearching(char theSearchKey[MAXN], log **theLogArray,int l, int r){
    int m = (l+r)/2;
    if(l>r)
        return -1;
    if(strcmp(theSearchKey,theLogArray[m]->partenza)==0)
        return m;
    if(strcmp(theSearchKey,theLogArray[m]->partenza)<0)
        return dichotomicSearching(theSearchKey,theLogArray,l,m-1);
    return dichotomicSearching(theSearchKey,theLogArray,m+1,r);

}

