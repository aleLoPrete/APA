#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#define MAXN 31


typedef struct {
    char idCode[MAXN];
    char partenza[MAXN];
    char arrivo[MAXN];
    char dataLog[MAXN];
    char StartingTime[MAXN];
    char ArrivalTime[MAXN];
    int delay;
}log;



int storeLog(FILE *thePnt, log *theLogArray);
void printLog(log *logArray, int start, int logLen, int cmd);
void printMenu();
int printLogStatus(int choice);
void sortLogPerDate(log *theLogArray, int theLogLen);
void sortLogPerTime(log *theLogArray, int theLogLen);
void sortLogPerCode(log *theLogArray, int theLogLen);
void sortLogPerPartenza(log *theLogArray, int theLogLen);
void sortLogPerArrivo(log *theLogArray, int theLogLen);
int dichotomicSearching(char theSearchKey[MAXN], log *theLogArray, int l, int r);
int dichotomicSearchingWrapper(char theSearchKey[MAXN], log *theLogArray, int len);
int linearSearching(char theSearchKey[MAXN], log *theLogArray, int logLen);


int main() {
    FILE *pnt;
    const char PATHfile[30] = "corse.txt";
    log logArray[1000];
    int logLen, choice=0, logStatus=0, cmd;
    char searchKey[MAXN];
    int matchingIndex;
    pnt = fopen(PATHfile, "r");

    if(pnt == NULL)
        return -1;

    logLen = storeLog(pnt,logArray);
    printf("%d", logLen);

    while(1) {
        printMenu();
        printLogStatus(logStatus);
        printf("\nscelta : \n");
        scanf("%d", &choice);

        switch(choice) {
            case -1:
                return 0;
            case 1:
                printf("inserire 1 per stampare a video, 0 per stampare su file(log.txt)");
                scanf("%d", &cmd);
                printLog(logArray, 0, logLen, cmd);
                break;
            case 2:
                sortLogPerTime(logArray,logLen);
                sortLogPerDate(logArray,logLen);
                logStatus = 2;
                break;
            case 3:
                sortLogPerCode(logArray, logLen);
                logStatus = 3;
                break;

            case 4:
                sortLogPerPartenza(logArray, logLen);
                logStatus = 4;
                break;
            case 5 :
                sortLogPerArrivo(logArray, logLen);
                logStatus = 5;
                break;
            case 6 :
                printf("\ninserisci la stazione di partenza come ricerca\n");
                scanf("%s", searchKey);
                if(logStatus == 4)
                    matchingIndex = dichotomicSearchingWrapper(searchKey, logArray, logLen);
                else
                    matchingIndex = linearSearching(searchKey, logArray, logLen);

                if(matchingIndex == -1) {
                    printf("\nchiave non trovata\n");
                    break;
                }
                printf("la chiave e' stata trovata nella tratta con indice %d:\n", matchingIndex);
                printLog(logArray, matchingIndex, 1, 1);
                break;

        }
    }

}


void printMenu(){
    printf("\n****MENU****\n");
    printf("-1>termina programma\n");
    printf("1>stampa log\n");
    printf("2>ordina per data\n");
    printf("3>ordina per codice di tratta\n");
    printf("4>ordina per stazione di partenza\n");
    printf("5>ordina per stazione di arrivo\n");
    printf("6>ricerca una tratta per stazione di partenza\n");
}

int printLogStatus(int choice) {

    if(choice == 2 || choice == 3 || choice == 4 || choice == 5){
        printf("log ordinato per : ");
        switch(choice) {
            case 2:
                printf("DATA\n");
                return 2;
            case 3:
                printf("CODICE TRATTA\n");
                return 3;
            case 4:
                printf("STAZIONE PARTENZA\n");
                return 4;
            case 5:
                printf("STAZIONE ARRIVO\n");
                return 5;
        }
        return 0;
    }
    printf("\nil log non e' stato ordinato\n");
    return 0;
}

int storeLog(FILE *thePnt, log *theLogArray){
    int theLogLen, i=0;
    fscanf(thePnt,"%d", &theLogLen);
    while( i<theLogLen){
        fscanf(thePnt,"%s %s %s %s %s %s %d", theLogArray[i].idCode, theLogArray[i].partenza, theLogArray[i].arrivo,
               (theLogArray[i].dataLog), theLogArray[i].StartingTime, theLogArray[i].ArrivalTime, &(theLogArray[i].delay));
        i++;
    };

    return theLogLen;
}

void printLog(log *logArray, int start, int logLen, int cmd){
    int i=start;
    FILE* fp;
    int r = i+logLen;
    if(cmd == 1){
        for(i=start; i<r; i++) {
            printf("\n%s %s %s %s %s -> %s delay: %d minuti\n", logArray[i].idCode, logArray[i].partenza, logArray[i].arrivo, logArray[i].dataLog,
                   logArray[i].StartingTime, logArray[i].ArrivalTime, logArray[i].delay);
        }
    }
    else{
        fp = fopen("log.txt", "w+");
        for(i=start; i<r; i++) {
            fprintf(fp, "%s %s %s %s %s -> %s delay: %d minuti\n", logArray[i].idCode, logArray[i].partenza, logArray[i].arrivo, logArray[i].dataLog,
                   logArray[i].StartingTime, logArray[i].ArrivalTime, logArray[i].delay);
        }
        fclose(fp);
    }
}

void sortLogPerDate(log *theLogArray, int theLogLen){
    log temp;
    for(int i =0; i<theLogLen; i++) {
        for(int l=0; l<theLogLen-1; l++){
            if(strcmp(theLogArray[l].dataLog,theLogArray[l+1].dataLog)>0){
                temp = theLogArray[l+1];
                theLogArray[l+1]=theLogArray[l];
                theLogArray[l] = temp;
            }
        }
    }
}

void sortLogPerTime(log *theLogArray, int theLogLen){
    log temp;
    for(int i =0; i<theLogLen; i++) {
        for(int l=0; l<theLogLen-1; l++){
            if(strcmp(theLogArray[l].StartingTime,theLogArray[l+1].StartingTime)>0){
                temp = theLogArray[l+1];
                theLogArray[l+1]=theLogArray[l];
                theLogArray[l] = temp;
            }
        }
    }
}

void sortLogPerCode(log *theLogArray, int theLogLen){
    log temp;
    for(int i =0; i<theLogLen; i++) {
        for(int l=0; l<theLogLen-1; l++){
            if(theLogArray[l].idCode>theLogArray[l+1].idCode){
                temp = theLogArray[l+1];
                theLogArray[l+1]=theLogArray[l];
                theLogArray[l] = temp;
            }
        }
    }
}

void sortLogPerPartenza(log *theLogArray, int theLogLen){
    log temp;
    for(int i =0; i<theLogLen; i++) {
        for(int l=0; l<theLogLen-1; l++){
            if(strcmp(theLogArray[l].partenza,theLogArray[l+1].partenza)>0){
                temp = theLogArray[l+1];
                theLogArray[l+1]=theLogArray[l];
                theLogArray[l] = temp;
            }
        }
    }
}

void sortLogPerArrivo(log *theLogArray, int theLogLen){
    log temp;
    for(int i =0; i<theLogLen; i++) {
        for(int l=0; l<theLogLen-1; l++){
            if(strcmp(theLogArray[l].arrivo,theLogArray[l+1].arrivo)>0){
                temp = theLogArray[l+1];
                theLogArray[l+1]=theLogArray[l];
                theLogArray[l] = temp;
            }
        }
    }
}

int dichotomicSearchingWrapper(char theSearchKey[MAXN], log *theLogArray, int len){
    int l =0;
    int r = len-1;
    return dichotomicSearching(theSearchKey,theLogArray,l,r);
}

int dichotomicSearching(char theSearchKey[MAXN], log *theLogArray,int l, int r){
    int m = (l+r)/2;
    if(l>r)
        return -1;
    if(strcmp(theSearchKey,theLogArray[m].partenza)==0)
        return m;
    if(strcmp(theSearchKey,theLogArray[m].partenza)<0)
        return dichotomicSearching(theSearchKey,theLogArray,l,m-1);
    return dichotomicSearching(theSearchKey,theLogArray,m+1,r);

}

int linearSearching(char theSearchKey[MAXN], log *theLogArray, int logLen){
    int i;
    for(i=0; i<logLen; i++) {
        if(strcmp(theLogArray[i].partenza,theSearchKey) == 0)
            return i;
    }
    return -1;
}

