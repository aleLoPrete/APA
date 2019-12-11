#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PATHPG "pg.txt"
#define PATHOB "inventario.txt"
enum {MAXL = 51, TXT = 1, VIDEO = 2}convinienceVar;

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
    int nInv;
    int maxInv;
}tabInv;


typedef struct{//EQUIPAGGIAMENTO PERSONAGGIO
    int inUse;
    object **arrEquip;
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
KEY *getKEY(pg *pg_t);
int KEYgreater(KEY *key1, KEY *key2);
link sortedPush(link head, pg *pg_t);
void printMenu();
void runSelectedFun(tabPg *tabPg_t, int choice);
int getPg(int stream, link theNodePg_t, link head);
void printList(link head);

//MAIN -----------------------------------------------------------------------------------------------------------------
int main(){
    tabPg tabPg_t;
    theNodePg theNodePg_t;
    int choice;

    tabPg_t.head = &theNodePg_t;
    tabPg_t.tail = &theNodePg_t;
    tabPg_t.nPg = 0;
    theNodePg_t.next = NULL;

    do{
        printMenu(&choice);
        scanf("%d", &choice);
        runSelectedFun(&tabPg_t, choice); //todo aggiornare tail quando aggiorno lista personaggi
    }while(choice != 0);
    return 0;
}

//FUNZIONI -------------------------------------------------------------------------------------------------------------
void printMenu(){
    printf("\n** M E N U **\n");
    printf("\n0>esci dal programma");
    printf("\n1>carica elenco di personaggi");
    printf("\n2>carica elenco di oggetti");
    printf("\n3>nuovo personaggio");
    printf("\n4>elimina personaggio");
    printf("\n5>modifica equipaggiamento di un personaggio");
    printf("\n6>mostra statistiche di un personaggio");
    printf("\n>>choice :  ");
}

KEY *getKEY(pg *pg_t){
    KEY *key = (KEY*)malloc(MAXL * sizeof(char));
    strcpy(key, pg_t->code);
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

link newNode(pg *pg_t, link next) {

    link x = malloc(sizeof(*x));
    if (x==NULL)
        return NULL; // controllo disponibilità memoria
    else {
        x->pg_t = *pg_t;
        x->next = next;
    }
    return x;
}

link sortedPush(link head, pg *pg_t){
    link x, p;
    KEY *k = getKEY(pg_t);
    KEY *headK = getKEY(&(head->pg_t));

    if (head == NULL || !KEYlessOrEq(k,headK)) // inserisco in ordine crescente
        return newNode(pg_t, head);
    for (x = head->next, p = head;
         x != NULL && KEYgreater(k,getKEY(&(x->pg_t)));
         p = x, x = x->next); // scorro la lista finché non trovo la posizione ordinata in cui inserire
    p->next = newNode(pg_t, x);
    return head;
}

void runSelectedFun(tabPg *tabPg_t, int choice){

    switch(choice){
        case 1:
            getPg(TXT,tabPg_t->head,tabPg_t->head); break;
        case 3:
            getPg(VIDEO,tabPg_t->head,tabPg_t->head); break;
        case 7:
            printList(tabPg_t->head); break;
        default: printf("\ncaso default\n");
    }
    return;
}

int getPg(int stream, link theNodePg_t, link head){
    FILE *fp;

    if(stream == TXT){//acquisisco da file
        printf("siamo dentro");
        fp = fopen("pg.txt","r");
        if(fp==NULL) {return -1;} printf("file aperto");
        while(fscanf(fp,"%s %s %s %d %d %d %d %d %d",theNodePg_t->pg_t.code, theNodePg_t->pg_t.name, theNodePg_t->pg_t.class,
        &theNodePg_t->pg_t.stats_t.hp, &theNodePg_t->pg_t.stats_t.mp, &theNodePg_t->pg_t.stats_t.atk, &theNodePg_t->pg_t.stats_t.def,
                      &theNodePg_t->pg_t.stats_t.mag, &theNodePg_t->pg_t.stats_t.spr) != EOF){
            head = sortedPush(head, &theNodePg_t->pg_t);
        }
        fclose(fp);
    }
    else if(stream == VIDEO){//acquisisco da standard input
        printf("\ninserisci personaggio - CODICE NOME CLASSE STATS(hp, mp, atk, def, mag, spr)");
        scanf("%s %s %s %d %d %d %d %d %d",theNodePg_t->pg_t.code, theNodePg_t->pg_t.name, theNodePg_t->pg_t.class,
              &theNodePg_t->pg_t.stats_t.hp, &theNodePg_t->pg_t.stats_t.mp, &theNodePg_t->pg_t.stats_t.atk, &theNodePg_t->pg_t.stats_t.def,
              &theNodePg_t->pg_t.stats_t.mag, &theNodePg_t->pg_t.stats_t.spr);
        head = sortedPush(head, &theNodePg_t->pg_t);
    }
    return 0;
}

void printList(link head){
    link x;
    x = head;
    while( x != NULL){
        printf("%s %s %s\n", x->pg_t.code, x->pg_t.name, x->pg_t.class);
        x = x->next;
    }
}