#include <stdio.h>
#include <stdlib.h>
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
    tile *possibleTiles;
    int tileNum;
    int numInUse;
}tileCollection;

typedef struct{
    tile *tilePointer;
    int rotation;
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
void findMax(boardWrap *theBoardWrap, tileCollection *theTileColl, int col, int row);
void enumPossibleTiles(tileCollection *theTileColl);
void printPossTileColl(tileCollection *theTileColl);
int evaluateScore(boardWrap *theBoardWrap);
int matchColor(char color1, char color2);
int boardScoreCalc(boardWrap *theBoardWrap, box **board);
void printBoardMax(boardWrap *theBoardWrap, box ***board);
//MAIN -----------------------------------------------------------------------------------------------------------------
int main() {
    tileCollection theTileColl;
    boardWrap theBoardWrap;
    tile tileInit;

    //acquisisco i dati
    getTileCollection("tiles.txt", &theTileColl);//acquisisco vettore piastrelle
    getBoardfFromFile("board.txt", &theBoardWrap, theTileColl);// acquisisco la board con la configurazione iniziale dal file
    printBoard(&theBoardWrap, &theBoardWrap.board);
    printf("\n%d %d",theBoardWrap.nRow, theBoardWrap.nCol);
    //inizializzo la soluzione massima
    tileInit.pipe1.val = 0;
    tileInit.pipe1.color = 'A';
    tileInit.pipe2.val = 0;
    tileInit.pipe2.color = 'A';
    solMaxInitaliazer(&theBoardWrap, &tileInit);//inizilizzo la board con soluzione massima
    //printTileColl(&theTileColl);

    //creo vettore di piastrelle in cui aggiungo, dopo ogni casella, la sua corrispondente ruotata
    enumPossibleTiles(&theTileColl);
    //printf("\n\n---possible tiles:\n");
    //printPossTileColl(&theTileColl);

    //ricerco la configurazione con lo score maggiore
    findMax(&theBoardWrap, &theTileColl, 0,0);
    printBoardMax(&theBoardWrap, &theBoardWrap.solMax);
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
    printf("%d %d",theBoardWrap->nRow, theBoardWrap->nCol);
    malloc2d(&theBoardWrap->board, theBoardWrap->nRow, theBoardWrap->nCol);
    for(int i = 0; i < theBoardWrap->nRow; i-=-1){
        for(int j = 0; j <theBoardWrap->nCol; j++){
            fscanf(fp,"%d/%d", &tileIndex, &theBoardWrap->board[i][j].rotation);
            theBoardWrap->board[i][j].tilePointer = NULL;
            if(tileIndex != -1){
                theBoardWrap->board[i][j].tilePointer = &theTileColl.theTiles[tileIndex];
                theTileColl.theTiles[tileIndex].inUse = 1;
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

void findMax(boardWrap *theBoardWrap, tileCollection *theTileColl, int col, int row){

    if(col == (theBoardWrap->nCol-1)){//se sono arrivato alla fine della riga
        if(row == (theBoardWrap->nRow -1)){//sono arrivato alla fine della scacchiera
            if(evaluateScore(theBoardWrap)){// se la soluzione è massima
                theBoardWrap->solMax = theBoardWrap->board; // copio la soluzione max
            }
            return;
        }
        findMax(theBoardWrap, theTileColl, 0, row+1);//ricorro sulla riga successiva, alla prima colonna
    }

    if(theBoardWrap->board[col][row].tilePointer != NULL){//se la casella è già occupata
            findMax(theBoardWrap, theTileColl, col+1, row); //ricorro sulla colonna successiva
    }
    else{//se la casella è vuota
        for(int i = 0; i < (theTileColl->tileNum * 2); i++){//ciclo che scorre tutte le possibili piastrelle inseribili
            if(theTileColl->possibleTiles[i].inUse == 0){//controllo se la posso mettere
                theTileColl->possibleTiles[i].inUse = 1;//la marco come inserita
                //inserimento piastrella
                theBoardWrap->board[col][row].tilePointer = &theTileColl->possibleTiles[i];//inserisco la piastrella
                findMax(theBoardWrap, theTileColl, col+1, row);//ricorro sulla colonna successiva
                theTileColl->possibleTiles[i].inUse = 0;//BACKTRACK
            }
        }
    }
}

int evaluateScore(boardWrap *theBoardWrap){//ritorna '1' se il valore attuale è maggiore del Max passato, '0' se non è così
    int scoreBoard1, scoreBoardMax;

    scoreBoardMax = boardScoreCalc(theBoardWrap, theBoardWrap->solMax);
    scoreBoard1 = boardScoreCalc(theBoardWrap, theBoardWrap->board);
    if(scoreBoard1 > scoreBoardMax) {
        return 1;
    }
    else
            return 0;
    }

int boardScoreCalc(boardWrap *theBoardWrap, box **board){//se passo la la board by reference ho un SIGSEGV, se la passo by value no
    int flagRowColor = 0, flagColColor = 0;
    int totScoreRow = 0;
    int totScoreCol = 0;
    int rowPoints[theBoardWrap->nRow], colPoints[theBoardWrap->nCol];

    for(int i = 0; i < theBoardWrap->nRow; i++){
        for(int j = 0; j < theBoardWrap->nCol; j++){
            //calcolo il punteggio per la riga
            if(j == 0){//inizio riga
                rowPoints[i] = board[i][j].tilePointer->pipe1.val;
                flagRowColor = 1;
            }
            else if(matchColor(theBoardWrap->board[i][j].tilePointer->pipe1.color,theBoardWrap->board[i][j-1].tilePointer->pipe1.color) && flagRowColor){
                rowPoints[i] += board[i][j].tilePointer->pipe1.val;
            }
            else{
                rowPoints[i] = 0;
                flagRowColor = 0;
            }
            //calcolo il punteggio per le colonne
            if(i == 0){//inizio colonna
                colPoints[j] = board[i][j].tilePointer->pipe2.val;
                flagColColor = 1;
            }
            else if(matchColor(theBoardWrap->board[i][j].tilePointer->pipe1.color,theBoardWrap->board[i-1][j].tilePointer->pipe1.color) && flagColColor){
                colPoints[j] += board[i][j].tilePointer->pipe2.val;
            }
            else {
                colPoints[j] = 0;
                flagColColor = 0;
            }
        }
    }
    for(int i = 0; i < theBoardWrap->nRow; i++){
        totScoreRow += rowPoints[i];
    }
    for(int j = 0; j < theBoardWrap->nCol; j++){
        totScoreCol *= colPoints[j];
    }
    return totScoreCol + totScoreRow;
}

int matchColor(char color1, char color2){//guarda che la casella prima sia dello stesso colore
    if(color1 == color2 ){
        return 1;
    }
    else
        return 0;
}

void enumPossibleTiles(tileCollection *theTileColl){

    theTileColl->possibleTiles = (tile *)malloc((theTileColl->tileNum * 2) * sizeof(tile));
    for(int i = 0, j = 0; i < (theTileColl->tileNum * 2); i++, j++){
        //corrispondente non ruotata
        theTileColl->possibleTiles[i].inUse = theTileColl->theTiles[j].inUse;
        theTileColl->possibleTiles[i].pipe1.val = theTileColl->theTiles[j].pipe1.val;
        theTileColl->possibleTiles[i].pipe1.color = theTileColl->theTiles[j].pipe1.color;
        theTileColl->possibleTiles[i].pipe2.val = theTileColl->theTiles[j].pipe2.val;
        theTileColl->possibleTiles[i].pipe2.color = theTileColl->theTiles[j].pipe2.color;
        i++;
        theTileColl->possibleTiles[i].inUse = theTileColl->theTiles[j].inUse;
        theTileColl->possibleTiles[i].pipe1.val = theTileColl->theTiles[j].pipe2.val;
        theTileColl->possibleTiles[i].pipe1.color = theTileColl->theTiles[j].pipe2.color;
        theTileColl->possibleTiles[i].pipe2.val = theTileColl->theTiles[j].pipe1.val;
        theTileColl->possibleTiles[i].pipe2.color = theTileColl->theTiles[j].pipe1.color;
    }
}

void printPossTileColl(tileCollection *theTileColl){
    for(int i = 0; i < theTileColl->tileNum*2; i++){
        printf("\ntile[%d] : %c %d %c %d (inUse: %d)", i, theTileColl->possibleTiles[i].pipe1.color, theTileColl->possibleTiles[i].pipe1.val,
               theTileColl->possibleTiles[i].pipe2.color, theTileColl->possibleTiles[i].pipe2.val, theTileColl->possibleTiles[i].inUse);
    }
}

