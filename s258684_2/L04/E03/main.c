#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXDIM 60

char *cercaRegexp(char *theString, char *regExp);
int countCar(char *theExp);
int verifyMatch(char *word, char *regExp);


int main(){
    char string[MAXDIM], regExp[MAXDIM], *c;

    printf("inserisci frase da esaminare:\n");
    fgets(string, MAXDIM, stdin);
    printf("\nInserire espressione regolare: ");
    scanf("%s", regExp);

    c = cercaRegexp(string, regExp);

    if (c == NULL)
        printf("\nEspressione non trovata.");
    else{
        sscanf(c,"%s",c);
        printf("\nla prima parola che corrisponde all'espressione regolare e' :%s", c);
    }


}


char *cercaRegexp(char *theString, char *regExp){
    char *pntRiga;
    char word[MAXDIM];
    int i, lenExp ;

    pntRiga = theString;

    lenExp = countCar(regExp);

    while(sscanf(pntRiga, "%s%n", word, &i) != EOF){
        if(lenExp == strlen(word)) {
            if(verifyMatch(word, regExp))
                return pntRiga;
        }
        pntRiga += i;
    }
    return NULL;
}

int countCar(char *theExp){
    int eInd = 0, counter = 0;

    while(eInd < strlen(theExp)){
        if(theExp[eInd]=='.'){
            eInd++;
            counter++;
            continue;
        }
        if(theExp[eInd]=='['){
            counter++;
            do{
                eInd++;
            }while(theExp[eInd]!= ']');
            eInd++;
            continue;
        }
        if(theExp[eInd]=='\\'){
            counter++;
            eInd += 2;
            continue;
        }
        eInd++;
        counter++;
    }
    return counter;
}

int verifyMatch(char *word, char *regExp) {
    int wIn=0, eIn=0, wLen = strlen(word), eLen = strlen(regExp), mismatch=0, match = 0;

    while(wIn < wLen && eIn < eLen && mismatch == 0) {
        mismatch = 1;
        switch(regExp[eIn]) {
            case '.' : // espressione regolare : ' . '
                mismatch = 0;
                wIn++;
                eIn++;
                break;
            case '[':
                eIn++;
                if(regExp[eIn]!= '^'){ // espressione regolare : ' [ ] '
                    do{
                        if(regExp[eIn]==word[wIn]){
                            mismatch = 0;
                            match = 1;
                            wIn++;
                            for( ; regExp[eIn] != ']'; ++eIn );
                        }
                        eIn++;
                    }while(regExp[eIn]!= ']' && match == 0);
                    break;
                }
                else{ // espressione regolare : ' [^ ] '
                    eIn++;
                    while(regExp[eIn] != ']'){
                        if(regExp[eIn] == word[wIn]){
                            mismatch = 1;
                            break;
                        }
                        else{
                            eIn++;
                        }
                    }
                    mismatch = 0;
                    eIn++;
                    wIn++;
                    break;
                }
            case '\\' :
                eIn++;
                if(regExp[eIn] == 'a'){ // espressione regolare: ' \a '
                    if(islower(word[wIn]) != 0){
                        eIn++;
                        wIn++;
                        mismatch = 0;
                    }
                    else
                        mismatch = 1;
                    break;
                }
                else if(regExp[eIn] == 'A'){ // espressione regolare: ' \A '
                    if(isupper(word[wIn]) != 0){
                        eIn++;
                        wIn++;
                        mismatch = 0;
                    }
                    else
                        mismatch = 1;
                    break;
                }

            default:
                if(regExp[eIn] == word[wIn]){
                    mismatch = 0;
                    wIn++;
                    eIn++;
                }
                break;
        }
    }

    if(mismatch == 0)
        return 1;
    else
        return 0;
}