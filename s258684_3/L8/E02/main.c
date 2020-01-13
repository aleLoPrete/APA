#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define MAXL 30

typedef struct{
    char color;
    int val;
}pipe;

typedef struct _tile tile;

struct _tile{
    pipe pipe1;
    pipe pipe2;
    int inUse;
};

typedef struct{
    tile *theTiles;
    int tileNum;
    int numInUse;
}tileCollection;

typedef struct{
    tile *tilePointer;
    int rotation;
    int removable;
}box;

typedef struct{
    box **board;
    box **solMax;
    int nCol;
    int nRow;
}boardWrap;

//PROTOTIPI FUNZIONI ---------------------------------------------------------------------------------------------------
void getTileCollection(char PATH[MAXL], tileCollection *theTileColl);
void malloc2d(box ***board, int row, int col);
void printTileColl(tileCollection *theTileColl);
void getBoardfFromFile(char PATH[MAXL], boardWrap *theBoardWrap, tileCollection theTileColl);
void solMaxInitaliazer(boardWrap *theBoardWrap, tile *tileInit);
void printBoard(boardWrap *theBoardWrap, box ***board);
void findMax(boardWrap *theBoardWrap, tileCollection *theTileColl, int col, int row, int *counter);
void copyBoard(boardWrap *theBoardWrap);
int evaluateScore(boardWrap *theBoardWrap);
int matchColor(box *box1, box *box2);
int boardScoreCalc(boardWrap *theBoardWrap, box **board);
void printBoardMax(boardWrap *theBoardWrap, box ***board);
//MAIN -----------------------------------------------------------------------------------------------------------------
int main() {
    tileCollection theTileColl;
    boardWrap theBoardWrap;
    tile tileInit;

    tileInit.pipe1.val = 0;
    tileInit.pipe1.color = 'A';
    tileInit.pipe2.val = 0;
    tileInit.pipe2.color = 'A';
    int *counter;
    int a = 0;
    counter = &a;
    //acquisisco i dati
    getTileCollection("tiles.txt", &theTileColl);//acquisisco vettore piastrelle
    getBoardfFromFile("board.txt", &theBoardWrap, theTileColl);// acquisisco la board con la configurazione iniziale dal file
    printBoard(&theBoardWrap, &theBoardWrap.board);

    //inizializzo la soluzione massima
    solMaxInitaliazer(&theBoardWrap, &tileInit);//inizilizzo la board con soluzione massima

    //ricerco la configurazione con lo score maggiore
    findMax(&theBoardWrap, &theTileColl, 0,0, counter);
    printf("\nSOLUZIONE FINALE:\n");
    printBoardMax(&theBoardWrap, &theBoardWrap.solMax);
    int max = boardScoreCalc(&theBoardWrap, theBoardWrap.solMax);
    printf("\nmax = %d\ncounter:%d", max, a);
    return 0;
}

//FUNZIONI -------------------------------------------------------------------------------------------------------------

void getTileCollection(char PATH[MAXL], tileCollection *theTileColl){
    FILE *fp;
    tileCollection tempTileColl;
    fp = fopen(PATH, "r");
    if(fp == NULL){printf("\nFile %s non trovato\n",PATH); return;}

    fscanf(fp,"%d", &tempTileColl.tileNum);
    tempTileColl.theTiles = (tile *)malloc(tempTileColl.tileNum * sizeof(tile));

    for(int i = 0; i < tempTileColl.tileNum; i++){
        fscanf(fp,"\n%c %d %c %d", &tempTileColl.theTiles[i].pipe1.color, &tempTileColl.theTiles[i].pipe1.val,
               &tempTileColl.theTiles[i].pipe2.color, &tempTileColl.theTiles[i].pipe2.val);
        tempTileColl.theTiles[i].inUse = 0;
    }
    *theTileColl = tempTileColl;
}

void printTileColl(tileCollection *theTileColl){
    for(int i = 0; i < theTileColl->tileNum; i++){
        printf("\ntile[%d] : %c %d %c %d (inUse: %d)", i, theTileColl->theTiles[i].pipe1.color, theTileColl->theTiles[i].pipe1.val,
                theTileColl->theTiles[i].pipe2.color, theTileColl->theTiles[i].pipe2.val, theTileColl->theTiles[i].inUse);
    }
}

void getBoardfFromFile(char PATH[MAXL], boardWrap *theBoardWrap, tileCollection theTileColl){
    FILE *fp;
    fp = fopen(PATH, "r");
    int tileIndex;
    if(fp == NULL){printf("\nFile %s non trovato\n",PATH); return;}
    fscanf(fp, "%d %d", &theBoardWrap->nRow, &theBoardWrap->nCol);
    malloc2d(&theBoardWrap->board, theBoardWrap->nRow, theBoardWrap->nCol);
    for(int i = 0; i < theBoardWrap->nRow; i-=-1){
        for(int j = 0; j <theBoardWrap->nCol; j++){
            fscanf(fp,"%d/%d", &tileIndex, &theBoardWrap->board[i][j].rotation);
            theBoardWrap->board[i][j].tilePointer = NULL;
            theBoardWrap->board[i][j].removable = 1;
            if(tileIndex != -1){
                theBoardWrap->board[i][j].tilePointer = &theTileColl.theTiles[tileIndex];
                theTileColl.theTiles[tileIndex].inUse = 1;
                theBoardWrap->board[i][j].removable = 0;
            }
        }
    }
}

void malloc2d(box ***board, int row, int col){
    box **temporaryBoard;
    temporaryBoard = (box **)malloc(row * sizeof(box *));
    for(int i = 0; i < col; i++){
        temporaryBoard[i] = (box *)malloc(col * sizeof(box));
    }
    *board = temporaryBoard;
}

void printBoard(boardWrap *theBoardWrap, box ***board){

    for(int i = 0; i < theBoardWrap->nRow; i++){
        for(int j = 0; j < theBoardWrap->nCol; j++){
            if(theBoardWrap->board[i][j].tilePointer != NULL ){
                printf("|%c%d%c%d r:%d|", theBoardWrap->board[i][j].tilePointer->pipe1.color, theBoardWrap->board[i][j].tilePointer->pipe1.val,
                       theBoardWrap->board[i][j].tilePointer->pipe2.color, theBoardWrap->board[i][j].tilePointer->pipe2.val,
                       theBoardWrap->board[i][j].rotation);
            }
            else
                printf("| ////// |");
        }
        printf("\n");
    }
}
void solMaxInitaliazer(boardWrap *theBoardWrap, tile *tileInit){//inizializza lo score della board massima a 0, tutto con lo stesso colore
    malloc2d(&theBoardWrap->solMax, theBoardWrap->nRow, theBoardWrap->nCol);
    for(int i = 0; i < theBoardWrap->nRow; i++){ // metto la stessa piastrella in tutta la board per avere uno score totale di 0
        for(int j = 0; j < theBoardWrap->nCol; j++ ){
            theBoardWrap->solMax[i][j].tilePointer = tileInit;
            theBoardWrap->solMax[i][j].rotation = 0;
        }
    }
}

void printBoardMax(boardWrap *theBoardWrap, box ***board){
    for(int i = 0; i < theBoardWrap->nRow; i++){
        for(int j = 0; j < theBoardWrap->nCol; j++){
            if(theBoardWrap->solMax[i][j].tilePointer != NULL ){
                printf("|%c%d%c%d r:%d|", theBoardWrap->solMax[i][j].tilePointer->pipe1.color, theBoardWrap->solMax[i][j].tilePointer->pipe1.val,
                       theBoardWrap->solMax[i][j].tilePointer->pipe2.color, theBoardWrap->solMax[i][j].tilePointer->pipe2.val,
                       theBoardWrap->solMax[i][j].rotation);
            }
            else
                printf("| ////// |");
        }
        printf("\n");
    }
}

void findMax(boardWrap *theBoardWrap, tileCollection *theTileColl, int col, int row, int *counter){
    if(col >= (theBoardWrap->nCol)){//se sono arrivato alla fine della riga
        if(row == (theBoardWrap->nRow -1)){//sono arrivato alla fine della scacchiera
            (*counter)++;
            evaluateScore(theBoardWrap);// valuta la soluzione e la salva se massima
            return;
        }
        findMax(theBoardWrap, theTileColl, 0, row+1, counter);//ricorro sulla riga successiva, alla prima colonna
        return;
    }
    if (theBoardWrap->board[col][row].tilePointer == NULL) {//se la casella è vuota
        for (int i = 0; i < theTileColl->tileNum; i++) {//ciclo che scorre tutte le piastrelle
            if (theTileColl->theTiles[i].inUse != 1) { // se non è in uso
                theTileColl->theTiles[i].inUse = 1;//la marco come inserita
                theBoardWrap->board[col][row].tilePointer = &theTileColl->theTiles[i];//inserisco la piastrella
                theBoardWrap->board[col][row].rotation = 0;//segno che l'ho inserita non ruotata
                findMax(theBoardWrap, theTileColl, col + 1, row, counter);//ricorro sulla colonna successiva
                theBoardWrap->board[col][row].rotation = 1; // segno che l'ho inserita ruotata
                findMax(theBoardWrap, theTileColl, col + 1, row, counter);//ricorro sulla colonna successiva
                theBoardWrap->board[col][row].tilePointer = NULL;
                theTileColl->theTiles[i].inUse = 0;
            }
        }
    } else {//se la casella è già occupata
        findMax(theBoardWrap, theTileColl, col + 1, row, counter); //ricorro sulla colonna successiva
        return;
    }
}

int evaluateScore(boardWrap *theBoardWrap){//ritorna '1' se il valore attuale è maggiore del Max passato, '0' se non è così
    int scoreBoard1, scoreBoardMax;

    scoreBoardMax = boardScoreCalc(theBoardWrap, theBoardWrap->solMax);
    scoreBoard1 = boardScoreCalc(theBoardWrap, theBoardWrap->board);
    if(scoreBoard1 > scoreBoardMax) {
        copyBoard(theBoardWrap);
        return 1;
    }
    else
        return 0;
}

void copyBoard(boardWrap *theBoardWrap){
    for(int i = 0; i < theBoardWrap->nRow; i++){
        for(int j = 0; j < theBoardWrap->nCol; j++){
            theBoardWrap->solMax[i][j]= theBoardWrap->board[i][j];
        }
    }
}

int boardScoreCalc(boardWrap *theBoardWrap, box **board){
    int streakRowColor = 0, streakColColor = 0;
    int totScoreRow = 0;
    int totScoreCol = 0;
    int rowPoints[theBoardWrap->nRow], colPoints[theBoardWrap->nCol];
    int flagMatch;

    //CALCOLO PUNTEGGIO RIGHE
    for(int i = 0; i < theBoardWrap->nRow; i++){
        for(int j = 0; j < theBoardWrap->nCol; j++){
            if(j == 0){//inizio riga
                if(board[i][j].rotation == 0){
                    rowPoints[i] = board[i][j].tilePointer->pipe1.val;
                }
                else{
                    rowPoints[i] = board[i][j].tilePointer->pipe2.val;
                }
                streakRowColor = 1;
            }
            else if(streakRowColor) {//se c'è streak di colore
                flagMatch = matchColor(&board[i][j], &board[i][j - 1]);
                if(flagMatch){
                    if(board[i][j].rotation == 0){
                        rowPoints[i] += board[i][j].tilePointer->pipe1.val;
                    }
                    else{
                        rowPoints[i] += board[i][j].tilePointer->pipe2.val;
                    }
                }
                else{
                    streakRowColor = 0;
                    rowPoints[i] = 0;
                }
            }
            else{
                rowPoints[i] = 0;
                streakRowColor = 0;
            }
        }
    }
    //CALCOLO PUNTEGGIO COLONNE
    for(int j = 0; j < theBoardWrap->nCol; j++){
        for(int i = 0; i < theBoardWrap->nRow; i++) {
            if(i == 0){//inizio colonna
                if(board[i][j].rotation == 0){
                    colPoints[j] = board[i][j].tilePointer->pipe2.val;
                }
                else{
                    colPoints[j] = board[i][j].tilePointer->pipe1.val;
                }
                streakColColor = 1;
            }
            else if(streakColColor) {//se c'è streak di colore
                flagMatch = matchColor(&board[i][j], &board[i - 1][j]);
                if (flagMatch) {
                    if(board[i][j].rotation == 0){
                        colPoints[j] += board[i][j].tilePointer->pipe2.val;
                    }
                    else{
                        colPoints[j] += board[i][j].tilePointer->pipe1.val;
                    }
                }
                else{
                    colPoints[j] = 0;
                    streakColColor = 0;
                }
            }
            else{
                colPoints[j] = 0;
                streakColColor = 0;
            }
        }
    }

    for(int i = 0; i < theBoardWrap->nRow; i++){
        totScoreRow += rowPoints[i];
    }
    for(int j = 0; j < theBoardWrap->nCol; j++){
        totScoreCol += colPoints[j];
    }
    return totScoreCol + totScoreRow;
}

int matchColor(box *box1, box *box2){//guarda che la casella prima sia dello stesso colore
    char color1, color2;
    if(box1->rotation == 0){
        color1 = box1->tilePointer->pipe1.color;
    }
    else{
        color1 = box1->tilePointer->pipe2.color;
    }

    if(box2->rotation == 0){
        color2 = box2->tilePointer->pipe1.color;
    }
    else{
        color2 = box2->tilePointer->pipe2.color;
    }

    if(color1 == color2){
        return 1;
    }
    else{
        return 0;
    }
}