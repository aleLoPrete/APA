#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXN 51

typedef char KEY;
typedef struct{
    char giorno[3];
    char mese[3];
    char anno[5];
}date;

typedef struct{
    char codice[MAXN];
    char nome[MAXN], cognome[MAXN];
    KEY nascita[MAXN];
    char via[MAXN];
    char city[MAXN];
    int cap;
}item;

typedef struct nodeDef node;
typedef struct nodeDef *link;
struct nodeDef{
    item val;
    link next;
};

//PROTOTIPI FUNZIONI --------------------------
int fmenu();
link callOp(int choice, link _head);
void printList(link head);
char *getDateRight(date *theDate);
node getNode(int choice);
KEY *getKEY(item val);
int KEYgreater(KEY *key, KEY *key2);
link sortedPush(link h, item val);
link newNode(item val, link next);
node *listSearch(link h, char *code);
node delAndExtr(link *h, char *code);

// MAIN ---------------------------------------
int main() {
    link head = NULL;
    int choice;
    while(choice!=0){
        choice = fmenu();
        head = callOp(choice, head);
    }
    return 0;
}

//DEFINIZIONI FUNZIONI ------------------------
int fmenu(){
    int choice;
        printf("\n**MENU**\n");
        printf("0>esci dal programma\n");
        printf("1>inserisci un nuovo elemento\n");
        printf("2>inserisci una serie di nuovi elementi\n");
        printf("3>ricerca di un elemento per chiave\n");
        printf("4>cancella(ed estrai) un elemento\n");
        printf("5>cancella un intervallo di elementi\n");
        printf("6>stampa lista\n");
        scanf("%d", &choice);
    return choice;
}

link callOp(int choice, link _head){
    node theNode_t, deleted; //node temporaneo da inserire in lista
    FILE *fp;
    date theDate, date1, date2;
    link match;
    char code[MAXN], date1r[MAXN], date2r[MAXN];
    char PATH[MAXN];
    link head = _head;
    int flag=0;

    switch(choice){
        case 0: //esco dal programma
            return head;
        case 1:
            theNode_t = getNode(choice);
            head = sortedPush(head, theNode_t.val);
            break;
        case 2:
            printf("inserisci la PATH del file con i dati da inserire");
            scanf("%s", PATH);
            fp = fopen(PATH,"r");
            if(fp==NULL) {return head;}
            while( fscanf(fp,"%s %s %s %s %s %s %s %s %d", theNode_t.val.codice, theNode_t.val.nome, theNode_t.val.cognome,
                    theDate.giorno, theDate.mese, theDate.anno, theNode_t.val.via, theNode_t.val.city, &theNode_t.val.cap) != EOF ){
                strcpy(theNode_t.val.nascita,getDateRight(&theDate));
                head = sortedPush(head,theNode_t.val);
            }
            fclose(fp);
            break;
        case 3:
            printf("Inserire codice:");
            scanf("%s", code);
            match = listSearch(head, code);
            match != NULL ? printf("elemento %s trovato!\n", match->val.codice) : printf("Non trovato.\n");
            break;
        case 4:
            printf("Inserire codice:");
            scanf("%s", code);
            match = listSearch(head, code);
            if(match != NULL){
                deleted = delAndExtr(&head,code);
                printf("Eliminazione avvenuta con successo : %s", deleted.val.codice);
            }
            else
                printf("\nnon trovato!\n");
            break;
        case 5:
            printf("\nInserire range date (<data_inizio> <data_fine>) nel formato <gg/mm/aaaa>:\n");
            scanf("%s %s %s %s %s %s", date1.giorno, date1.mese, date1.anno, date2.giorno, date2.mese, date2.anno );
            strcpy(date1r, getDateRight(&date1));
            strcpy(date2r, getDateRight(&date2));
            link x = head;
            while( x != NULL){
                if(KEYgreater(date1r, x->val.nascita) && KEYgreater(x->val.nascita, date2r)){
                    if((match = listSearch(head,x->val.codice)) != NULL){
                        flag = 1;
                        deleted = delAndExtr(&head,x->val.codice);
                        printf("\n%s eliminato\n", deleted.val.codice);
                    }
                }
                x = x->next;
            }
            if (flag == 0)
                printf("\nrange di date non presente in anagrafica\n");
            break;
        case 6:
            printList(head);
            break;
    }
    return head;
}

node getNode(int choice){
    node theNode;
    date theDate;
    if(choice == 1){
        printf(">codice:");
        scanf("%s", theNode.val.codice);
        printf("\n>nome:");
        scanf("%s", theNode.val.nome);
        printf("\n>cognome:");
        scanf("%s", theNode.val.cognome);
        printf("\n>data di nascita (giorno/mese/anno):");
        scanf("%s%s%s", theDate.giorno, theDate.mese, theDate.anno);
        strcpy(theNode.val.nascita,getDateRight(&theDate));
        printf("\n>via:");
        scanf("%s", theNode.val.via);
        printf("\n>citta':");
        scanf("%s", theNode.val.city);
        printf("\n>CAP");
        scanf("%d", &theNode.val.cap);

        return theNode;
    }
    if(choice==2){
        printf("in fase di sviluppo");

        return theNode;
    }
}

char *getDateRight(date *theDate){
    char *rightDate;
    rightDate = (char *)malloc(50*sizeof(char));
    strcpy(rightDate, theDate->anno);
    strcat(rightDate, "/");
    strcat(rightDate, theDate->mese);
    strcat(rightDate, "/");
    strcat(rightDate, theDate->giorno);
    return rightDate;
}

KEY *getKEY(item val){
    KEY *key = (KEY*)malloc(20*sizeof(char));
    strcpy(key, val.nascita);
    return key;
}

int KEYgreater(KEY *key, KEY *key2){
    if(strcmp(key,key2)<=0)
        return 1;
    return 0;
}

link newNode(item val, link next) {

    link x = malloc(sizeof(*x));
    if (x==NULL)
        return NULL; // controllo disponibilità memoria
    else {
        x->val = val;
        x->next = next;
    }
    return x;
}

link sortedPush(link h, item val){
    link x, p;
    KEY *k = getKEY(val);

    if (h==NULL || KEYgreater(getKEY(h->val),k))
        return newNode(val, h);
    for (x=h->next, p=h;
         x!=NULL && KEYgreater(k,getKEY(x->val));
         p=x, x=x->next);
    p->next = newNode(val, x);
    return h;
}

void printList(link head){
    FILE *fp = fopen("list.txt", "w+");
    link x;
    x = head;
    while( x != NULL){
        fprintf(fp,"%s %s %s %s %s %s %d\n", x->val.codice, x->val.nome, x->val.cognome, x->val.nascita, x->val.via, x->val.city, x->val.cap);
        x = x->next;
    }
}

node *listSearch(link h, char *code) {
    link x;
    for (x = h; x != NULL; x = x->next) {
        if (strcmp(code, x->val.codice) == 0)
            return x;
    }
    return NULL;
}

node delAndExtr(link *h, char *code) {
    link *xp, t;
    node i;
    for (xp = h; (*xp) != NULL; xp = &((*xp)->next)) {
        if (strcmp((*xp)->val.codice, code) == 0) {
            t = *xp;
            *xp = (*xp)->next;
            i = *t;
            free(t);
            break;
        }
    }
    return i;
}