//
// Created by Alessandro on 28/12/2019.
//

#include "pgList.h"
struct _nodePg{ //NODO LISTA PERSONAGGI
    pg pg_t;
    link next;
};

struct _tabPg{//WRAPPER LISTA PERSONAGGI
    link head;
    link tail;
    int nPg;
};

KEY *getKEY(pg val){
    KEY *key = (KEY*)malloc(MAXL * sizeof(char));
    strcpy(key, val.code);
    return key;
}

void initTabPg(tabPg *tabPg_t){
    tabPg_t->head = NULL;
    tabPg_t->nPg = 0;
}

int KEYgreater(KEY *key1, KEY *key2){//vero se la prima chiave è strettamente maggiore della seconda
    if(strcmp(key1,key2) > 0) return 1 ;
    else return 0 ;
}

int KEYlessOrEq(KEY *key1, KEY *key2){//vero se la prima chiave è strettamente maggiore della seconda
    if(strcmp(key1,key2) <= 0) return 1 ;
    else return 0 ;
}

link newNode(pg val, link next){

    link x = malloc(sizeof(*x));
    if (x==NULL)
        return NULL; // controllo disponibilità memoria
    else {
        x->pg_t = val;
        x->next = next;
    }
    return x;
}

link sortedPush(link head, pg val){
    link x, p;
    KEY *k = getKEY(val);
    KEY *headK = getKEY(val);

    if (head == NULL || !KEYlessOrEq(k,headK)) // inserisco in ordine crescente
        return newNode(val, head);
    for (x = head->next, p = head;
         x != NULL && KEYgreater(k,getKEY(x->pg_t));
         p = x, x = x->next); // scorro la lista finché non trovo la posizione ordinata in cui inserire
    p->next = newNode(val, x);
    return head;
}

link getPg(int stream, tabPg *tabPg1){
    FILE *fp;
    theNodePg theNodePg_t;
    link head = tabPg1->head;
    int hp, mp, atk, def, mag, spr;
    if(stream == TXT){//acquisisco da file
        fp = fopen(PATHPG,"r");
        if(fp==NULL) {return head;}
        while(fscanf(fp,"%s %s %s %d %d %d %d %d %d",theNodePg_t.pg_t.code, theNodePg_t.pg_t.name, theNodePg_t.pg_t.class,
                     &hp, &mp, &atk, &def, &mag, &spr) != EOF){
            setStats(&(theNodePg_t.pg_t.stats_t), hp, mp, atk, def, mag, spr);
            setVoidInUse(theNodePg_t.pg_t.equip_t);
            initEquip(theNodePg_t.pg_t.equip_t);
            head = sortedPush(head, theNodePg_t. pg_t);
        }
        fclose(fp);
    }
    else if(stream == VIDEO){//acquisisco da standard input
        printf("\ninserisci personaggio - CODICE NOME CLASSE STATS(hp, mp, atk, def, mag, spr)");
        scanf("%s %s %s %d %d %d %d %d %d",theNodePg_t.pg_t.code, theNodePg_t.pg_t.name, theNodePg_t.pg_t.class,
              &hp, &mp, &atk, &def, &mag, &spr);
        setStats(&theNodePg_t.pg_t.stats_t, hp, mp, atk, def, mag, spr);
        setVoidInUse(theNodePg_t.pg_t.equip_t);
        initEquip(theNodePg_t.pg_t.equip_t);
        head = sortedPush(head, theNodePg_t.pg_t);
    }
    tabPg1->head = head;
    return head;
}

void printList(tabPg *tabPg_t, tabInv *tabInvP){
    link x;
    x = tabPg_t->head;
    printf("\n");
    printf("*****************LISTA PG IN MEMORIA***************\n");
    while( x != NULL){
        printf("%s %s %s  %dhp %dmp %datk %ddef %dmag %dspr\n", x->pg_t.code, x->pg_t.name, x->pg_t.class, x->pg_t.stats_t.hp,
               x->pg_t.stats_t.mp, x->pg_t.stats_t.atk, x->pg_t.stats_t.def, x->pg_t.stats_t.mag, x->pg_t.stats_t.spr);
        printf("\teq:");
        printEquipPg(tabPg_t->head->pg_t.equip_t,tabInvP);
        printf("\n");
        x = x->next;
    }
    printf("****************************************************");
}
void pgExtraction(tabPg *tabPg_t){
    char toBeDeleted[MAXL];
    theNodePg deleted;
    int flag = 0;
    printf("\nInserisci il codice del personaggio da eliminare");
    scanf("%s", toBeDeleted);
    deleted = extraction(&tabPg_t->head, toBeDeleted, &flag);
    if(flag == 1){
        printf("\nPersonaggio %s eliminato con successo!", deleted.pg_t.code);
    }
    else printf("\nIl codice inserito non ha corrispondenze all'interno della lista PG!\n");

}

theNodePg extraction(link *h, char *code, int *flag){
    link *xp, t;
    theNodePg deletedPg;

    for (xp = h; (*xp) != NULL; xp = &((*xp)->next)) {
        if (strcmp((*xp)->pg_t.code, code) == 0) {
            t = *xp;
            *xp = (*xp)->next;
            deletedPg = *t;
            free(t);
            *flag = 1;
            break;
        }
    }
    return deletedPg;
}

void addObj2Pg(tabPg *tabPg_t, tabInv *tabInv_t){
    char codePG[MAXL], nameObj[MAXL];
    int j, objIndex;
    link *pgPointer;
    int choice;
    int numInUse;

    printf("\n1>aggiungi un oggetto\n2>rimuovi un oggetto");
    scanf("%d", &choice);

    printf("\nInserisci codice personaggio:");
    scanf("%s", codePG);
    pgPointer = findPg(codePG, &tabPg_t->head);//trovo personaggio
    if (pgPointer == NULL) {
        printf("\nPersonaggio non trovato!");
        return;
    }
    numInUse = inUse((*pgPointer)->pg_t.equip_t);
    //agiungi oggetto
    if(choice==1) {

        if (numInUse == MAXEQ) {
            printf("\nequipaggiamento peronaggio completo! non è possibile aggiungere oggetti :(");
            return;
        }
        objIndex = addObjToEquip((*pgPointer)->pg_t.equip_t,tabInv_t);
        increaseInUse((*pgPointer)->pg_t.equip_t);
        printf("oggetto i.%d aggiunto all'equipaggiamento di ", objIndex, (*pgPointer)->pg_t.name);
    }
    //rimuovi oggetto
    else if(choice == 2){
        if(numInUse == 0){
            printf("\nIl personaggio non ha oggetti equipaggiati!");
            return;
        }
        printf("\nInserisci nome oggetto da rimuovere:");
        scanf("%s", nameObj);
        objIndex = removeObjFromEq((*pgPointer)->pg_t.equip_t, nameObj,tabInv_t);
        printf("oggetto i.%d rimosso dall'equipaggiamento di '%s'", objIndex,
               (*pgPointer)->pg_t.name);
    }
}

link *findPg(char codePG[MAXL], link *head){
    link *x;
    for(x = head; (*x) != NULL; x = &(*x)->next){
        if(strcmp((*x)->pg_t.code, codePG) == 0) {
            return x;
        }
    }
    return NULL;
}

void statsCalc(tabPg *tabPg_t, tabInv *tabInvP){
    char codePg[MAXL];
    link *pgPointer;
    stats stats1;
    int i;

    printf("\ninserisci codice PG:");
    scanf("%s", codePg);
    pgPointer = findPg(codePg, &tabPg_t->head);
    if(pgPointer == NULL)
        return;
    stats1 = (*pgPointer)->pg_t.stats_t;//inizializzo stats1

    for(i = 0; i < MAXEQ; i++){//scorro l'array dell'equipaggiamento del personaggio

        if(getObjInd((*pgPointer)->pg_t.equip_t, i) != -1){//se c'è un oggetto nella posizione esaminata, aggiungo le stats

            if(stats1.hp + getStatFromObject(i,1,(*pgPointer)->pg_t.equip_t, tabInvP) >= 1){
                stats1.hp += getStatFromObject(i,1,(*pgPointer)->pg_t.equip_t, tabInvP);
            }
            else{ stats1.hp = 1;}

            if(stats1.atk + getStatFromObject(i,2,(*pgPointer)->pg_t.equip_t, tabInvP) >= 1){
                stats1.atk += getStatFromObject(i,2,(*pgPointer)->pg_t.equip_t, tabInvP);
            }
            else{ stats1.atk = 1;}

            if(stats1.def + getStatFromObject(i,3,(*pgPointer)->pg_t.equip_t, tabInvP) >= 1){
                stats1.def += getStatFromObject(i,3,(*pgPointer)->pg_t.equip_t, tabInvP) ;
            }
            else{ stats1.def = 1;}

            if(stats1.mag + getStatFromObject(i,4,(*pgPointer)->pg_t.equip_t, tabInvP) >= 1){
                stats1.mag += getStatFromObject(i,4,(*pgPointer)->pg_t.equip_t, tabInvP);
            }
            else{ stats1.mag = 1;}

            if(stats1.mp + getStatFromObject(i,5,(*pgPointer)->pg_t.equip_t, tabInvP) >= 1){
                stats1.mp += getStatFromObject(i,5,(*pgPointer)->pg_t.equip_t, tabInvP);
            }
            else{ stats1.mp = 1;}

            if(stats1.spr + getStatFromObject(i,6,(*pgPointer)->pg_t.equip_t, tabInvP) >= 1){
                stats1.spr += getStatFromObject(i,6,(*pgPointer)->pg_t.equip_t, tabInvP);
            }
            else{ stats1.spr = 1;}
        }
    }

    printf("\nPG '%s' - %dhp %dmp %datk %ddef %dmag %dspr", (*pgPointer)->pg_t.name, stats1.hp, stats1.mp,
           stats1.atk, stats1.def, stats1.mag, stats1.spr );
}