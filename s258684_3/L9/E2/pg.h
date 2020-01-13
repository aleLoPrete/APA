//
// Created by Alessandro on 28/12/2019.
//

#ifndef ES02_PG_H
#define ES02_PG_H
#include "equipArray.h"
#include "invArray.h"
#include "inv.h"

typedef struct{//PERSONAGGIO
    char code[MAXL];
    char name[MAXL];
    char class[MAXL];
    equip *equip_t;
    stats stats_t;
}pg;

#endif //ES02_PG_H
