//
// Created by Alessandro on 15/12/2019.
//

#ifndef ES02_INV_H
#define ES02_INV_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXL 51
#define MAXEQ 8
#define PATHOB "inventario.txt"

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

void printInv(object *array);
object *storeObj(tabInv *tabInv_t);
int findObj(char *nameObj, tabInv *tabInv_t);

#endif //ES02_INV_H
