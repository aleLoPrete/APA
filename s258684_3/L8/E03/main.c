#include "pg.h"


void printMenu(int *choice);
void runSelectedFun(tabPg *tabPg_t,tabInv *tabInv_t, int choice);

int main() { // todo controllare eliminazione prsoaggio e modifica eq
    tabPg tabPg_t;
    tabInv tabInv_t;
    int choice;

    tabPg_t.head = NULL;
    tabPg_t.nPg = 0;

    do{
        printMenu(&choice);
        runSelectedFun(&tabPg_t, &tabInv_t, choice);
    }while(choice != 0);
    return 0;
}

void printMenu(int *choice){
    printf("\n******************* M E N U ************************");
    printf("\n*  0>esci dal programma                            *");
    printf("\n*  1>carica elenco di personaggi                   *");
    printf("\n*  2>carica elenco di oggetti                      *");
    printf("\n*  3>nuovo personaggio                             *");
    printf("\n*  4>elimina personaggio                           *");
    printf("\n*  5>modifica equipaggiamento di un personaggio    *");
    printf("\n*  6>mostra statistiche di un personaggio          *");
    printf("\n*  7>stampa lista personaggi                       *");
    printf("\n*  8>stampa lista oggetti                          *");
    printf("\n****************************************************");
    printf("\n>>choice :");
    scanf("%d", choice);
}

void runSelectedFun(tabPg *tabPg_t, tabInv *tabInv_t, int choice){

    switch(choice){
        case 1:
            tabPg_t->head = getPg(TXT,tabPg_t->head);
            break;
        case 2:
            tabInv_t->arrInv = storeObj(tabInv_t);
            break;
        case 3:
            tabPg_t->head = getPg(VIDEO,tabPg_t->head);
            break;
        case 4:
            pgExtraction(tabPg_t->head);
            break;
        case 5:
            addObj2Pg(&tabPg_t->head,tabInv_t);
            break;
        case 6:
            statsCalc(tabPg_t);
            break;
        case 7:
            printList(tabPg_t->head);
            break;
        case 8:
            printInv(tabInv_t->arrInv);
    }
    return;
}
