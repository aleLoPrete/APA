#include <stdio.h>
#include <string.h>
#include <ctype.h>

enum  costanti{
    MAXsequenza = 6,
    MAXparola = 26,
    seqRow = 20,
    seqCol = 6,
};

int wordFix();


int main() {
    FILE *pntSeq;
    FILE *pntTesto;
    const char *PATHSeq = "sequenze.txt";
    const char *PATHTesto = "testo.txt";
    int seqLength, indiceRiga = 0, wordPos;
    char seq[MAXsequenza], sequenze[seqRow][seqCol], testoWord[MAXparola], firstWord[MAXparola], secondWord[MAXparola];


    pntSeq = fopen(PATHSeq, "r");
    if(pntSeq == NULL) {
        printf("\nerrore apertura file sequenze\n");
        return -1;
    }

    fscanf(pntSeq,"%d", &seqLength);

    // acquisico le sequenze e le memorizzo in un vettore di stringhe

    while(fscanf(pntSeq, "%s", seq) == 1){
        strcpy(sequenze[indiceRiga], seq);
        indiceRiga++;
    }
    // trasformo tutti i caratteri delle sequenze nell'alfabeto minuscolo
    for(int i=0; i< seqRow; i++) {
        for(int j=0; j<seqCol; j++) {
            sequenze[i][j] = tolower(sequenze[i][j]);
        }
    }
    // leggo il file parola per parola



    for (indiceRiga = 0; indiceRiga < seqLength; indiceRiga++) { //Per ogni sequenza in sequenze[][]

        pntTesto = fopen(PATHTesto, "r");
        if(pntTesto==NULL) {
            printf("errore apertura file testo.txt");
            return 2;
        }

        printf("\nla sequenza '%s' si trova nelle parole :\n ", sequenze[indiceRiga]);
        wordPos = 0;
        while ( fscanf(pntTesto, "%s", testoWord) == 1) {// scandisco ogni parola del testo,

            wordPos++;
            if(wordFix(testoWord)!=0) { // se la parola letta dal testo sono in realtà due parole
                //memorizzo le due parole in due variabili separate per poter ricercare la sequenza separatamente e aumentare il contatore correttamente
                for(int i=0; i<strlen(testoWord); i++) {
                    if(!isspace(testoWord[i])) {
                        firstWord[i] = testoWord[i];
                    }
                    else {
                        for(int j = i+1; j<(strlen(testoWord) - strlen(firstWord) +1 ); j++, i++) {
                            secondWord[j] = testoWord[i+1];
                        }
                        break;
                    }
                }
                //confronto separatamente le due parole ed eventualmente stampo se trovo il match
                if(strstr(firstWord, sequenze[indiceRiga]) != NULL ) {
                    printf(" <<%s>> (parola in posizione %d nel testo), ", firstWord, wordPos);
                    wordPos++;
                }
                else
                    wordPos++;// in ogni caso alla fine del controllo nella prima parola aumento il contatore delle parole nel testo

                if(strstr(secondWord, sequenze[indiceRiga]) != NULL ) {
                    printf(" <<%s>> (parola in posizione %d nel testo), ", secondWord, wordPos);
                    wordPos++;
                }

            }

            else {
                if (strstr(testoWord, sequenze[indiceRiga]) != NULL) {    // verificado se c'è la sequenza.
                    printf(" <<%s>> (parola in posizione %d nel testo), ", testoWord, wordPos);
                    //se contiene la squenza stampo a video ogni parola che la contiene indicando la posisione della parola nel testo
                }
                // se non contiene la sequenza passo a esaminare la parola successiva
            }
        }
        printf("\n");

        fclose(pntTesto);
    }

    return 0;
}

int wordFix(char parola[]) { // trasforma le lettere in minuscole e sostituisce le punteggiature di collegamento con uno spazio
                                // se inserisce uno spazio ritorna l'indice successivo al carattere spazio nella parola
    int flag=0, index=0;

    for(int i= 0; i<strlen(parola); i++) {
        if(!ispunct(parola[i])) {
            parola[i] = tolower(parola[i]);
        }
        else {
            if(parola[i+1] == '\0') {
                parola[i] = '\0';
            }
            else {
                parola[i] = ' ';
                flag=1;
            }
        }
    }

    if(flag)
        return 42;
    else
        return 0;
}
