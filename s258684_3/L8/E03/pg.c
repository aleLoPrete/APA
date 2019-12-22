//
// Created by Alessandro on 15/12/2019.
//

#include "pg.h"

KEY *getKEY(pg val){
    KEY *key = (KEY*)malloc(MAXL * sizeof(char));
    strcpy(key, val.code);
    return key;
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

link getPg(int stream, link _head){
    FILE *fp;
    theNodePg theNodePg_t;
    link head = _head; //todo risolvere dubbio, passagio come puntatore head non funziona, se la copio e la ritorno invece sì
    if(stream == TXT){//acquisisco da file
        fp = fopen(PATHPG,"r");
        if(fp==NULL) {return head;}
        while(fscanf(fp,"%s %s %s %d %d %d %d %d %d",theNodePg_t.pg_t.code, theNodePg_t.pg_t.name, theNodePg_t.pg_t.class,
                     &theNodePg_t.pg_t.stats_t.hp, &theNodePg_t.pg_t.stats_t.mp, &theNodePg_t.pg_t.stats_t.atk, &theNodePg_t.pg_t.stats_t.def,
                     &theNodePg_t.pg_t.stats_t.mag, &theNodePg_t.pg_t.stats_t.spr) != EOF){
            theNodePg_t.pg_t.equip_t.inUse = 0;
            for(int i=0; i < MAXEQ; i++){
                theNodePg_t.pg_t.equip_t.arrEquip[i] = NULL;
            }
            head = sortedPush(head, theNodePg_t. pg_t);
        }
        fclose(fp);
    }
    else if(stream == VIDEO){//acquisisco da standard input
        printf("\ninserisci personaggio - CODICE NOME CLASSE STATS(hp, mp, atk, def, mag, spr)");
        scanf("%s %s %s %d %d %d %d %d %d",theNodePg_t.pg_t.code, theNodePg_t.pg_t.name, theNodePg_t.pg_t.class,
              &theNodePg_t.pg_t.stats_t.hp, &theNodePg_t.pg_t.stats_t.mp, &theNodePg_t.pg_t.stats_t.atk, &theNodePg_t.pg_t.stats_t.def,
              &theNodePg_t.pg_t.stats_t.mag, &theNodePg_t.pg_t.stats_t.spr);
        theNodePg_t.pg_t.equip_t.inUse = 0;
        for(int i=0; i < MAXEQ; i++){
            theNodePg_t.pg_t.equip_t.arrEquip[i] = NULL;
        }
        head = sortedPush(head, theNodePg_t.pg_t);
    }
    return head;
}

void printList(link head){
    link x;
    x = head;
    printf("\n");
    printf("*****************LISTA PG IN MEMORIA***************\n");
    while( x != NULL){
        printf("%s %s %s  %dhp %dmp %datk %ddef %dmag %dspr\n", x->pg_t.code, x->pg_t.name, x->pg_t.class, x->pg_t.stats_t.hp,
               x->pg_t.stats_t.mp, x->pg_t.stats_t.atk, x->pg_t.stats_t.def, x->pg_t.stats_t.mag, x->pg_t.stats_t.spr);
        printf("\teq:");
        for(int i = 0; i < MAXEQ; i++){
            x->pg_t.equip_t.arrEquip[i] != NULL ? printf(" %s", x->pg_t.equip_t.arrEquip[i]->name) : printf(" X ");
        }
        printf("\n");
        x = x->next;
    }
    printf("****************************************************");
}
void pgExtraction(link head){
    char toBeDeleted[MAXL];
    theNodePg deleted;
    int flag = 0;
    printf("\nInserisci il codice del personaggio da eliminare");
    scanf("%s", toBeDeleted);
    deleted = extraction(&head, toBeDeleted, &flag);
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

void addObj2Pg(link *head, tabInv *tabInv_t){
    char codePG[MAXL], nameObj[MAXL];
    int j, objIndex;
    link *pgPointer;
    int flag = 0;
    int choice;

    printf("\n1>aggiungi un oggetto\n2>rimuovi un oggetto");
    scanf("%d", &choice);

    printf("\nInserisci codice personaggio:");
    scanf("%s", codePG);
    pgPointer = findPg(codePG, head);//trovo personaggio
    if (pgPointer == NULL) {
        printf("\npersonaggio non trovato!");
        return;
    }
    if(choice==1) {
        if ((*pgPointer)->pg_t.equip_t.inUse == MAXEQ) {
            printf("\nequipaggiamento peronaggio completo! non è possibile aggiungere oggetti :(");
            return;
        }
        for (j = 0; (*pgPointer)->pg_t.equip_t.arrEquip[j] != NULL && j < MAXEQ; j++);//cerco spazio libero nell'equipaggiamento
        printf("\nInserisci nome oggetto da aggiungere:");
        scanf("%s", nameObj);
        objIndex = findObj(nameObj, tabInv_t);//trovo oggetto
        if (objIndex == -1) {
            printf("\noggetto non trovato!");
            return;
        }
        (*pgPointer)->pg_t.equip_t.inUse++;
        (*pgPointer)->pg_t.equip_t.arrEquip[j] = &tabInv_t->arrInv[objIndex];//assegno all'array equipaggiamento del personaggio l'indirizzo dell'oggetto
        printf("oggetto '%s' aggiunto all'equipaggiamento di '%s'", tabInv_t->arrInv[objIndex].name,
               (*pgPointer)->pg_t.name);
    }

    else if(choice == 2){
        if((*pgPointer)->pg_t.equip_t.inUse == 0){
            printf("\nIl personaggio non ha oggetti equipaggiati!");
            return;
        }
        printf("\nInserisci nome oggetto da rimuovere:");
        scanf("%s", nameObj);
        for (j = 0; j < MAXEQ; j++){
            if((*pgPointer)->pg_t.equip_t.arrEquip[j] != NULL && strcmp((*pgPointer)->pg_t.equip_t.arrEquip[j]->name, nameObj) == 0 ){
                (*pgPointer)->pg_t.equip_t.arrEquip[j] = NULL ;//elimino oggetto dall'equip
                (*pgPointer)->pg_t.equip_t.inUse--; //dimiuisco il numero di oggetti in uso
                flag = 1;
                break;
            }
        }
        if(!flag) {printf("\nOggetto non trovato!"); return;}
        printf("oggetto '%s' rimosso dall'equipaggiamento di '%s'", tabInv_t->arrInv[j].name,
               (*pgPointer)->pg_t.name);
    }
}

link *findPg(char codePG[MAXL], link *head){
    link x;
    for(x = *head; x != NULL; x = x->next){
        if(strcmp(x->pg_t.code, codePG) == 0) {
            return x;
        }
    }
    printf("\nPersonaggio non trovato!\n");
    return NULL;
}

void statsCalc(tabPg *tabPg_t){
    char codePg[MAXL];
    link pgPointer;
    stats stats1;
    int i;

    printf("\ninserisci codice PG:");
    scanf("%s", codePg);
    pgPointer = findPg(codePg, &tabPg_t->head);
    if(pgPointer == NULL)
        return;
    stats1 = pgPointer->pg_t.stats_t;//inizializzo stats1

    for(i = 0; i < MAXEQ; i++){
        if(pgPointer->pg_t.equip_t.arrEquip[i] != NULL){
            if(stats1.hp + pgPointer->pg_t.equip_t.arrEquip[i]->stat_t.hp >= 1){
                stats1.hp += pgPointer->pg_t.equip_t.arrEquip[i]->stat_t.hp;
            }
            else{ stats1.hp = 1;}

            if(stats1.atk + pgPointer->pg_t.equip_t.arrEquip[i]->stat_t.atk >= 1){
                stats1.atk += pgPointer->pg_t.equip_t.arrEquip[i]->stat_t.atk;
            }
            else{ stats1.atk = 1;}

            if(stats1.def + pgPointer->pg_t.equip_t.arrEquip[i]->stat_t.def >= 1){
                stats1.def += pgPointer->pg_t.equip_t.arrEquip[i]->stat_t.def ;
            }
            else{ stats1.def = 1;}

            if(stats1.mag + pgPointer->pg_t.equip_t.arrEquip[i]->stat_t.mag >= 1){
                stats1.mag += pgPointer->pg_t.equip_t.arrEquip[i]->stat_t.mag;
            }
            else{ stats1.mag = 1;}

            if(stats1.mp + pgPointer->pg_t.equip_t.arrEquip[i]->stat_t.mp >= 1){
                stats1.mp += pgPointer->pg_t.equip_t.arrEquip[i]->stat_t.mp;
            }
            else{ stats1.mp = 1;}

            if(stats1.spr + pgPointer->pg_t.equip_t.arrEquip[i]->stat_t.spr >= 1){
                stats1.spr += pgPointer->pg_t.equip_t.arrEquip[i]->stat_t.spr;
            }
            else{ stats1.spr = 1;}
        }
    }

    printf("\nPG '%s' - %dhp %dmp %datk %ddef %dmag %dspr", pgPointer->pg_t.name, stats1.hp, stats1.mp,
           stats1.atk, stats1.def, stats1.mag, stats1.spr );
}