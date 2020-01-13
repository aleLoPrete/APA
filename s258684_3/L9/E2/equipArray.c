//
// Created by Alessandro on 28/12/2019.
//

#include "equipArray.h"

struct _equip{//EQUIPAGGIAMENTO PERSONAGGIO
    int inUse;
    int arrEquip[MAXEQ];
};

int inUse(equip *equipPnt){
    return equipPnt->inUse;
}

void setInUse(equip *equipPnt, int number){
    equipPnt->inUse = number;
}

void increaseInUse(equip *equipPnt){
    equipPnt->inUse++;
}

void setVoidInUse(equip *equipPnt){
    equipPnt->inUse = 0;
}

void initEquip(equip *equipPnt){
    for(int i = 0; i < MAXEQ; i++){
        equipPnt->arrEquip[i] = -1;
    }
}

int addObjToEquip(equip *equipPnt, tabInv *tabInv_t){
    char nameObj[MAXL];
    int objIndex;
    int j;

    printf("\nInserisci nome oggetto da aggiungere:");
    scanf("%s", nameObj);
    objIndex = findObj(nameObj, tabInv_t);//trovo oggetto
    if (objIndex == -1) {
        printf("\noggetto non trovato!");
        return -1;
    }

    for ( j = 0; equipPnt->arrEquip[j] != -1 && j < MAXEQ; j++);//cerco spazio libero nell'equipaggiamento
    equipPnt->arrEquip[j] = objIndex;//assegno all'array equipaggiamento del personaggio l'indirizzo dell'oggetto
    increaseInUse(equipPnt);
    return objIndex;
}

int removeObjFromEq(equip *equipPnt, char nameObj[MAXL], tabInv *tabInv_t){
    int j;
    int objIndex;
    objIndex = findObj(nameObj, tabInv_t);//trovo oggetto
    if(objIndex != -1){
        equipPnt->arrEquip[objIndex] = -1;
        return objIndex;
    }

    printf("\nOgetto non trovato!");
    return -1;
}

void printEquipPg(equip *equipPnt, tabInv *tabInvP){
    char objName[MAXEQ];
    for(int i = 0; i < MAXEQ; i++){
        if(equipPnt->arrEquip[i] == -1){
            printf(" X ");
        }
        else{
            strcpy(objName, getObjName(tabInvP,equipPnt->arrEquip[i]));
            printf(" %s ", objName);
        }
    }
}

int getStatFromObject(int indexArrEquip, int statIndex, equip *equipP, tabInv *tabInvP){
    return getStat(equipP->arrEquip[indexArrEquip], tabInvP, statIndex);
}

int getObjInd(equip *equipPnt, int i){
    return equipPnt->arrEquip[i];
}