//
// Created by Alessandro on 28/12/2019.
//

#ifndef ES02_INV_H
#define ES02_INV_H
#define MAXL 51
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct { // STATISTICHE OGGETTO/PERSONAGGIO
    int hp, mp, atk, def, mag, spr;
}stats;

typedef struct{ // OGGETTO
    char name[MAXL];
    char type[MAXL];
    stats stat_t;
}object;

#endif //ES02_INV_H
