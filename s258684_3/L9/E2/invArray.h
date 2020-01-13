//
// Created by Alessandro on 28/12/2019.
//

#ifndef ES02_INVARRAY_H
#define ES02_INVARRAY_H
#include "inv.h"
#define MAXEQ 8
#define PATHOB "inventario.txt"

typedef struct _tabInv tabInv;// STRUTTURA WRAPPER TABELLA INVENTARIO

//stampa dell'inventario
void printInv(tabInv *tabInv_t);

//input oggetto
object *storeObj(tabInv *tabInv_t);
//ritorna il nome dell'oggetto cercato
char *getObjName(tabInv *tabInvP, int objIndex);
//cerca oggetto per chiave data
int findObj(char *nameObj, tabInv *tabInv_t);

//funzioni di input e get delle statistiche
void setStats(stats *statsP, int hp, int mp, int atk, int def, int mag, int spr);
int getStat(int objIndex, tabInv *tabInvP, int statIndex);

#endif //ES02_INVARRAY_H
