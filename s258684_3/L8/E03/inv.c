    //
// Created by Alessandro on 15/12/2019.
//

#include "inv.h"
void printInv(object *array){
    for(int i= 0; i<15; i++){
        printf("\n%s %s", array[i].name, array[i].type);
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