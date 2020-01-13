//
// Created by Alessandro on 28/12/2019.
//

#ifndef ES02_EQUIPARRAY_H
#define ES02_EQUIPARRAY_H
#include "invArray.h"

typedef struct _equip equip;//struttura equipaggiamento personaggio

int inUse(equip *equipPnt);
void setInUse(equip *equipPnt, int number);
void increaseInUse(equip *equipPnt);
void setVoidInUse(equip *equipPnt);
int addObjToEquip(equip *equipPnt, tabInv *tabInv_t);
int removeObjFromEq(equip *equipPnt, char nameObj[MAXL], tabInv *tabInv_t);
void printEquipPg(equip *equipPnt, tabInv *tabInvP);
int getStatFromObject(int indexArrEquip, int statIndex, equip *equipP, tabInv *tabInvP);
void initEquip(equip *equipPnt);
int getObjInd(equip *equipPnt, int i);


#endif //ES02_EQUIPARRAY_H
