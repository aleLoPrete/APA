//
// Created by Alessandro on 15/12/2019.
//

#ifndef ES02_PG_H
#define ES02_PG_H
#include "inv.h"
#define PATHPG "pg.txt"
enum {TXT = 1, VIDEO = 2}convinienceVar;
typedef char KEY;

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


KEY *getKEY(pg val);
int KEYgreater(KEY *key1, KEY *key2);
int KEYlessOrEq(KEY *key1, KEY *key2);
link sortedPush(link head, pg val);
link newNode(pg val, link next);
link getPg(int stream, link head);
void printList(link head);
void pgExtraction(link head);
theNodePg extraction(link *h, char *code, int *flag);
void addObj2Pg(link *head, tabInv *tabInv_t);
link *findPg(char codePG[MAXL], link *head);
void statsCalc(tabPg *tabPg_t);
#endif //ES02_PG_H
