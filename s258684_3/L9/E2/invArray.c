//
// Created by Alessandro on 28/12/2019.
//

#include "invArray.h"

struct _tabInv{ // STRUTTURA WRAPPER TABELLA INVENTARIO
    object *arrInv;
    int lenInv;
    int maxInv;
};

void printInv(tabInv *tabInv_t){
    for(int i= 0; i<15; i++){
        printf("\n%s %s", tabInv_t->arrInv[i].name, tabInv_t->arrInv[i].type);
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
    tabInv_t->arrInv = array;
    return array;
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

char *getObjName(tabInv *tabInvP, int objIndex){
    return tabInvP->arrInv[objIndex].name;
}


void setStats(stats *statsP, int hp, int mp, int atk, int def, int mag, int spr){
    statsP->hp = hp;
    statsP->mp = mp;
    statsP->atk = atk;
    statsP->def = def;
    statsP->mag = mag;
    statsP->spr = spr;
    return;
}

int getStat(int objIndex, struct _tabInv *tabInvP, int statIndex){

    switch(statIndex){
        case 1:
            return tabInvP->arrInv[objIndex].stat_t.hp;
        case 2:
            return tabInvP->arrInv[objIndex].stat_t.mp;
        case 3:
            return tabInvP->arrInv[objIndex].stat_t.hp;
        case 4:
            return tabInvP->arrInv[objIndex].stat_t.atk;
        case 5:
            return tabInvP->arrInv[objIndex].stat_t.mag;
        case 6:
            return tabInvP->arrInv[objIndex].stat_t.spr;
    }
}
