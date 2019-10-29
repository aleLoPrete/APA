#include <stdio.h>
#include <string.h>
#include <ctype.h>



void encode(char parola[], const char dicPath[],FILE *pntEncode);

void wordEncoder(char parola[], int index, char match[], char codifica[], char toBePrinted[] );
int main() {

    FILE *pntSor, *pntEcd;
    const char *PATHsorgente = "C:\\Users\\Alessandro\\Documents\\POLI\\APA\\lab\\svolgimenti\\lab01\\es02\\sorgente.txt";
    const char *PATHdizionario = "C:\\Users\\Alessandro\\Documents\\POLI\\APA\\lab\\svolgimenti\\lab01\\es02\\dizionario.txt";
    const char *PATHricodifica = "C:\\Users\\Alessandro\\Documents\\POLI\\APA\\lab\\svolgimenti\\lab01\\es02\\ricodifica.txt";
    char parola[20];
    char riga[200];
    char *pntRiga;
    int lineMAX = 201;

    pntSor = fopen(PATHsorgente, "r");

    if(pntSor == NULL ) {
        return -1;
    }

    pntEcd = fopen(PATHricodifica, "w"); //[DEBUG] cancello i dati eventualmente presenti nel file di ricodifica
    fclose(pntEcd);


    pntEcd = fopen(PATHricodifica, "a");

    if(pntEcd == NULL ) {
        return -2;
    }

    while( fgets(riga, lineMAX, pntSor) != NULL) {
        //TODO sistemare --> se c'è più di uno spazio tra le parola il puntatore non ne tiene conto e si sposta come se ce ne fosse uno
        pntRiga = riga;
        while(sscanf(pntRiga,"%s", parola) == 1 ) {
            printf("%s ", parola); ////[DEBUG]

            encode(parola, PATHdizionario, pntEcd);
            pntRiga += (strlen(parola)+1);
        }
        printf("\n"); ////[DEBUG]
        fprintf(pntEcd,"\n");

    }

    fclose(pntEcd);

    return 0;

}


void encode(char parola[],const char dicPath[],FILE *pntEncode ) {

    FILE *pntDic;

    int i, lenDic, found = 0, startInd;
    char *index;
    char codifica[30], originale[30], toBePrinted[30];

    pntDic = fopen(dicPath, "r" );

    if(pntDic == NULL ) {
        printf("\nerrore apertura file dizionario.txt\n");
        return;
    }

    fscanf(pntDic, "%d", &lenDic); //leggo l'intero all'inizio del dizionario

    for( i=0; i<lenDic && !found ; i++) { // per la lunghezza trovata scandisco il diziionario leggendo <codifica> <originale>
        fscanf(pntDic, "%s %s", codifica, originale);

        index = strstr(parola, originale); // utilizzo la funzione strstr per sapere se <originale> si trova nella parola presa da sorgente.txt, e a che indice inzia la corrispondenza

        if(index != NULL) { // se trovo una corrispondenza non devo più scorrere il dizionario ed esco dal for
            found = 1;
        }
    }

    if( found == 0 ) { // se non è stata trovata nessuna corrispondenza tra la parola esaminata e le stringhe del dizionario, copio la parola del file sorgente
        strcpy(toBePrinted, parola);
    } else { // altrimenti cosrtuisco la parola codificata
        startInd = index - parola;
        wordEncoder(parola, startInd, originale, codifica, toBePrinted);
    }


    fprintf(pntEncode, "%s ", toBePrinted);

    return;

}



void wordEncoder(char parola[], int index, char match[], char codifica[], char toBePrinted[]) {
    int k, m, j;
    int totLengthSoFar, totWordRead;


    for(m = 0;  m < index ; m++ ) {
        toBePrinted[m] = parola[m];
    }

    for(k=0; k < strlen(codifica); k++ ) {
        toBePrinted[k+m] = codifica[k];
    }

    totLengthSoFar = index + strlen(codifica);
    totWordRead = index + strlen(match);

    for(m = totWordRead; m<strlen(parola); m++, totLengthSoFar++) {
        toBePrinted[totLengthSoFar] = parola[m];
    }
    toBePrinted[totLengthSoFar] = '\0';


    return;

}