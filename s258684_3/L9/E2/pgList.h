//
// Created by Alessandro on 28/12/2019.
//

#ifndef ES02_PGLIST_H
#define ES02_PGLIST_H
#include "pg.h"
#define PATHPG "pg.txt"

enum {TXT = 1, VIDEO = 2}convinienceVar;
typedef char KEY;

typedef struct _nodePg nodePg, theNodePg, *link;;
typedef struct _tabPg tabPg;

//funzioni per gestire la KEY
KEY *getKEY(pg val);
int KEYgreater(KEY *key1, KEY *key2);
int KEYlessOrEq(KEY *key1, KEY *key2);

//gestione lista
link sortedPush(link head, pg val);
link newNode(pg val, link next);
void pgExtraction(tabPg *tabPg1);
theNodePg extraction(link *h, char *code, int *flag);
link *findPg(char codePG[MAXL], link *head);

//input/output/extraction lista
link getPg(int stream, tabPg * tabPg1);//
void printList(tabPg *tabPg_t, tabInv *tabInvP);

//aggiunta oggetto a personaggio
void addObj2Pg(tabPg *tabPg_t, tabInv *tabInv_t);
//calcolo statistiche personaggio con oggetto
void statsCalc(tabPg *tabPg_t, tabInv *tabInvP);
//inizializzazione tabella Pg
void initTabPg(tabPg *tabPg_t);
#endif //ES02_PGLIST_H
