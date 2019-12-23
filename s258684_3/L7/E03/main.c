#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PATHPG "pg.txt"
#define PATHOB "inventario.txt"
enum {MAXL = 51, TXT = 1, VIDEO = 2, MAXEQ = 8}convinienceVar;

typedef char KEY;

typedef struct { // STATISTICHE OGGETTO/PERSONAGGIO
    int hp, mp, atk, def, mag, spr;
}stats;

typedef struct{ // OGGETTO
    char name[MAXL];
    char type[MAXL];
    stats stat_t;
}object;

typedef struct{ // STRUTTURA WRAPPER TABELLA INVENTARIO
    object *arrInv;
    int lenInv;
    int maxInv;
}tabInv;


typedef struct{//EQUIPAGGIAMENTO PERSONAGGIO
    int inUse;
    object *arrEquip[MAXEQ];
}equip;

typedef struct{//PERSONAGGIO
    char code[MAXL];
    char name[MAXL];
    char class[MAXL];
    equip equip_t;//nodePg->pg_t->equip_t->arrEquip[1]->name in questo modo accedo al nome del secondo oggetto equipaggiato da un pg
    stats stats_t;
}pg;

typedef struct nodePg theNodePg, *link;
struct nodePg{ //NODO LISTA PERSONAGGI
    pg pg_t;
    link next;
};

typedef struct{//WRAPPER LISTA PERSONAGGI
    link head;
    link tail;
    int nPg;
}tabPg;

//PROTOTIPI FUNZIONI ---------------------------------------------------------------------------------------------------
KEY *getKEY(pg val);
int KEYgreater(KEY *key1, KEY *key2);
int KEYlessOrEq(KEY *key1, KEY *key2);
void printMenu(int *choice);
void runSelectedFun(tabPg *tabPg_t,tabInv *tabInv_t, int choice);
link sortedPush(link head, pg val);
link newNode(pg val, link next);
link getPg(int stream, link head);
void printList(link head);
void printInv(object *array);
object *storeObj(tabInv *tabInv_t);
void pgExtraction(link *head);
theNodePg extraction(link *h, char *code, int *flag);
void addObj2Pg(link *head, tabInv *tabInv_t);
link *findPg(char codePG[MAXL], link *head);
int findObj(char *nameObj, tabInv *tabInv_t);
void statsCalc(tabPg *tabPg_t);
//MAIN -----------------------------------------------------------------------------------------------------------------
int main(){
    tabPg tabPg_t;
    tabInv tabInv_t;
    int choice;

    tabPg_t.head = NULL;
    tabPg_t.nPg = 0;

    do{
        printMenu(&choice);
        runSelectedFun(&tabPg_t, &tabInv_t, choice);
    }while(choice != 0);
    return 0;
}

//FUNZIONI -------------------------------------------------------------------------------------------------------------
void printMenu(int *choice){
    printf("\n******************* M E N U ************************");
    printf("\n*  0>esci dal programma                            *");
    printf("\n*  1>carica elenco di personaggi                   *");
    printf("\n*  2>carica elenco di oggetti                      *");
    printf("\n*  3>nuovo personaggio                             *");
    printf("\n*  4>elimina personaggio                           *");
    printf("\n*  5>modifica equipaggiamento di un personaggio    *");
    printf("\n*  6>mostra statistiche di un personaggio          *");
    printf("\n*  7>stampa lista personaggi                       *");
    printf("\n*  8>stampa lista oggetti                          *");
    printf("\n****************************************************");
    printf("\n>>choice :");
    scanf("%d", choice);
}

KEY *getKEY(pg val){
    KEY *key = (KEY*)malloc(MAXL * sizeof(char));
    strcpy(key, val.code);
    return key;
}

int KEYgreater(KEY *key1, KEY *key2){//vero se la prima chiave è strettamente maggiore della seconda
    if(strcmp(key1,key2) > 0) return 1 ;
    else return 0 ;
}

int KEYlessOrEq(KEY *key1, KEY *key2){//vero se la prima chiave è strettamente maggiore della seconda
    if(strcmp(key1,key2) <= 0) return 1 ;
    else return 0 ;
}

link newNode(pg val, link next){

    link x = malloc(sizeof(*x));
    if (x==NULL)
        return NULL; // controllo disponibilità memoria
    else {
        x->pg_t = val;
        x->next = next;
    }
    return x;
}

link sortedPush(link head, pg val){
    link x, p;
    KEY *k = getKEY(val);
    KEY *headK = getKEY(val);

    if (head == NULL || !KEYlessOrEq(k,headK)) // inserisco in ordine crescente
        return newNode(val, head);
    for (x = head->next, p = head;
         x != NULL && KEYgreater(k,getKEY(x->pg_t));
         p = x, x = x->next); // scorro la lista finché non trovo la posizione ordinata in cui inserire
    p->next = newNode(val, x);
    return head;
}

void runSelectedFun(tabPg *tabPg_t, tabInv *tabInv_t, int choice){

    switch(choice){
        case 1:
            tabPg_t->head = getPg(TXT,tabPg_t->head);
            break;
        case 2:
            tabInv_t->arrInv = storeObj(tabInv_t);
            break;
        case 3:
            tabPg_t->head = getPg(VIDEO,tabPg_t->head);
            break;
        case 4:
            pgExtraction(&tabPg_t->head);
            break;
        case 5:
            addObj2Pg(&tabPg_t->head,tabInv_t);
            break;
        case 6:
            statsCalc(tabPg_t);
            break;
        case 7:
            printList(tabPg_t->head);
            break;
        case 8:
            printInv(tabInv_t->arrInv);
    }
    return;
}

link getPg(int stream, link _head){
    FILE *fp;
    theNodePg theNodePg_t;
    link head = _head; //todo risolvere dubbio, passagio come puntatore head non funziona, se la copio e la ritorno invece sì
    if(stream == TXT){//acquisisco da file
        fp = fopen(PATHPG,"r");
        if(fp==NULL) {return head;}
        while(fscanf(fp,"%s %s %s %d %d %d %d %d %d",theNodePg_t.pg_t.code, theNodePg_t.pg_t.name, theNodePg_t.pg_t.class,
        &theNodePg_t.pg_t.stats_t.hp, &theNodePg_t.pg_t.stats_t.mp, &theNodePg_t.pg_t.stats_t.atk, &theNodePg_t.pg_t.stats_t.def,
                      &theNodePg_t.pg_t.stats_t.mag, &theNodePg_t.pg_t.stats_t.spr) != EOF){
            theNodePg_t.pg_t.equip_t.inUse = 0;
            for(int i=0; i < MAXEQ; i++){
                theNodePg_t.pg_t.equip_t.arrEquip[i] = NULL;
            }
            head = sortedPush(head, theNodePg_t. pg_t);
        }
        fclose(fp);
    }
    else if(stream == VIDEO){//acquisisco da standard input
        printf("\ninserisci personaggio - CODICE NOME CLASSE STATS(hp, mp, atk, def, mag, spr)");
        scanf("%s %s %s %d %d %d %d %d %d",theNodePg_t.pg_t.code, theNodePg_t.pg_t.name, theNodePg_t.pg_t.class,
              &theNodePg_t.pg_t.stats_t.hp, &theNodePg_t.pg_t.stats_t.mp, &theNodePg_t.pg_t.stats_t.atk, &theNodePg_t.pg_t.stats_t.def,
              &theNodePg_t.pg_t.stats_t.mag, &theNodePg_t.pg_t.stats_t.spr);
        theNodePg_t.pg_t.equip_t.inUse = 0;
        for(int i=0; i < MAXEQ; i++){
            theNodePg_t.pg_t.equip_t.arrEquip[i] = NULL;
        }
        head = sortedPush(head, theNodePg_t.pg_t);
    }
    return head;
}

void printList(link head){
    link *x;
    x = &head;
    printf("\n");
        printf("*****************LISTA PG IN MEMORIA***************\n");
    while( *x != NULL){
        printf("%s %s %s  %dhp %dmp %datk %ddef %dmag %dspr\n", (*x)->pg_t.code, (*x)->pg_t.name, (*x)->pg_t.class, (*x)->pg_t.stats_t.hp,
               (*x)->pg_t.stats_t.mp, (*x)->pg_t.stats_t.atk, (*x)->pg_t.stats_t.def, (*x)->pg_t.stats_t.mag, (*x)->pg_t.stats_t.spr);
        printf("\teq:");
        for(int i = 0; i < MAXEQ; i++){
            (*x)->pg_t.equip_t.arrEquip[i] != NULL ? printf(" %s", (*x)->pg_t.equip_t.arrEquip[i]->name) : printf(" X ");
        }
        printf("\n");
        x = &(*x)->next;
    }
        printf("****************************************************");
}

void printInv(object *array){
    for(int i= 0; i<15; i++){
        printf("\n%s %s", array[i].name, array[i].type);
    }
}

object *storeObj(tabInv *tabInv_t){
    object *array;
    int i;
    FILE *fp;

    fp = fopen(PATHOB, "r");
    if(fp == NULL){printf("\nFILE INVENTARIO NON TROVATO\n"); return NULL;}
    fscanf(fp,"%d", &tabInv_t->lenInv);
    array = (object *)malloc(tabInv_t->lenInv * sizeof(object));

    for(i = 0; i < tabInv_t->lenInv; i++){
        fscanf(fp,"%s %s %d %d %d %d %d %d",array[i].name, array[i].type, &array[i].stat_t.hp, &array[i].stat_t.mp,
                &array[i].stat_t.atk, &array[i].stat_t.def,  &array[i].stat_t.mag,  &array[i].stat_t.spr);
    }
    return array;
}

void pgExtraction(link *head){//eliminarlo dalla lista effettivamente
    char toBeDeleted[MAXL];
    theNodePg deleted;
    int flag = 0;
    printf("\nInserisci il codice del personaggio da eliminare");
    scanf("%s", toBeDeleted);
    deleted = extraction(head, toBeDeleted, &flag);
    if(flag == 1){
        printf("\nPersonaggio %s eliminato con successo!", deleted.pg_t.code);
    }
    else printf("\nIl codice inserito non ha corrispondenze all'interno della lista PG!\n");

}

theNodePg extraction(link *head, char *code, int *flag){
    link *xp,t;
    theNodePg deletedPg = {0};

    for (xp = head; xp != NULL; xp = &(*xp)->next) {
        if (strcmp((*xp)->pg_t.code, code) == 0) {
            t = (*xp);
            *xp = (*xp)->next;//eliminazione
            deletedPg = *t;
            free(t);
            *flag = 1;
            break;
        }
    }
    return deletedPg;
}

void addObj2Pg(link *head, tabInv *tabInv_t){
    char codePG[MAXL], nameObj[MAXL];
    int j, objIndex;
    link *pgPointer;
    int flag = 0;
    int choice;

    printf("\n1>aggiungi un oggetto\n2>rimuovi un oggetto");
    scanf("%d", &choice);

    printf("\nInserisci codice personaggio:");
    scanf("%s", codePG);
    pgPointer = findPg(codePG, head);//trovo personaggio
    if (pgPointer == NULL) {
        printf("\npersonaggio non trovato!");
        return;
    }
    if(choice==1) {
        if ((*pgPointer)->pg_t.equip_t.inUse == MAXEQ) {
            printf("\nequipaggiamento peronaggio completo! non è possibile aggiungere oggetti :(");
            return;
        }
        for (j = 0; (*pgPointer)->pg_t.equip_t.arrEquip[j] != NULL && j < MAXEQ; j++);//cerco spazio libero nell'equipaggiamento
        printf("\nInserisci nome oggetto da aggiungere:");
        scanf("%s", nameObj);
        objIndex = findObj(nameObj, tabInv_t);//trovo oggetto
        if (objIndex == -1) {
            printf("\noggetto non trovato!");
            return;
        }
        (*pgPointer)->pg_t.equip_t.inUse++;
        (*pgPointer)->pg_t.equip_t.arrEquip[j] = &tabInv_t->arrInv[objIndex];//assegno all'array equipaggiamento del personaggio l'indirizzo dell'oggetto
        printf("oggetto '%s' aggiunto all'equipaggiamento di '%s'", tabInv_t->arrInv[objIndex].name,
               (*pgPointer)->pg_t.name);
    }

    else if(choice == 2){
        if((*pgPointer)->pg_t.equip_t.inUse == 0){
            printf("\nIl personaggio non ha oggetti equipaggiati!");
            return;
        }
        printf("\nInserisci nome oggetto da rimuovere:");
        scanf("%s", nameObj);
        for (j = 0; j < MAXEQ; j++){
            if((*pgPointer)->pg_t.equip_t.arrEquip[j] != NULL && strcmp((*pgPointer)->pg_t.equip_t.arrEquip[j]->name, nameObj) == 0 ){
                (*pgPointer)->pg_t.equip_t.arrEquip[j] = NULL ;//elimino oggetto dall'equip
                (*pgPointer)->pg_t.equip_t.inUse--; //dimiuisco il numero di oggetti in uso
                flag = 1;
                break;
            }
        }
        if(!flag) {printf("\nOggetto non trovato!"); return;}
        printf("oggetto '%s' rimosso dall'equipaggiamento di '%s'", tabInv_t->arrInv[j].name,
               (*pgPointer)->pg_t.name);
    }
}

int findObj(char *nameObj, tabInv *tabInv_t){
    int i;
    for(i =  0; i < tabInv_t->lenInv; i++ ){
        if(strcmp(tabInv_t->arrInv[i].name,nameObj) == 0) {
          return i;
        }
    }
    return -1;
}

link *findPg(char codePG[MAXL], link *head){
    link *x;
    for(x = head; (*x) != NULL; x = &(*x)->next){
        if(strcmp((*x)->pg_t.code, codePG) == 0) {
            return x;
        }
    }
    return NULL;
}

void statsCalc(tabPg *tabPg_t){
    char codePg[MAXL];
    link pgPointer;
    stats stats1;
    int i;

    printf("\ninserisci codice PG:");
    scanf("%s", codePg);
    pgPointer = *findPg(codePg, &tabPg_t->head);
    stats1 = pgPointer->pg_t.stats_t;//inizializzo stats1

    for(i = 0; i < MAXEQ; i++){
        if(pgPointer->pg_t.equip_t.arrEquip[i] != NULL){
            if(stats1.hp + pgPointer->pg_t.equip_t.arrEquip[i]->stat_t.hp >= 1){
                stats1.hp += pgPointer->pg_t.equip_t.arrEquip[i]->stat_t.hp;
            }
            else{ stats1.hp = 1;}

            if(stats1.atk + pgPointer->pg_t.equip_t.arrEquip[i]->stat_t.atk >= 1){
                stats1.atk += pgPointer->pg_t.equip_t.arrEquip[i]->stat_t.atk;
            }
            else{ stats1.atk = 1;}

            if(stats1.def + pgPointer->pg_t.equip_t.arrEquip[i]->stat_t.def >= 1){
                stats1.def += pgPointer->pg_t.equip_t.arrEquip[i]->stat_t.def ;
            }
            else{ stats1.def = 1;}

            if(stats1.mag + pgPointer->pg_t.equip_t.arrEquip[i]->stat_t.mag >= 1){
                stats1.mag += pgPointer->pg_t.equip_t.arrEquip[i]->stat_t.mag;
            }
            else{ stats1.mag = 1;}

            if(stats1.mp + pgPointer->pg_t.equip_t.arrEquip[i]->stat_t.mp >= 1){
                stats1.mp += pgPointer->pg_t.equip_t.arrEquip[i]->stat_t.mp;
            }
            else{ stats1.mp = 1;}

            if(stats1.spr + pgPointer->pg_t.equip_t.arrEquip[i]->stat_t.spr >= 1){
                stats1.spr += pgPointer->pg_t.equip_t.arrEquip[i]->stat_t.spr;
            }
            else{ stats1.spr = 1;}
        }
    }

    printf("\nStatistiche con oggetti di '%s' %dhp %dmp %datk %ddef %dmag %dspr", pgPointer->pg_t.name, stats1.hp, stats1.mp,
            stats1.atk, stats1.def, stats1.mag, stats1.spr );
}
