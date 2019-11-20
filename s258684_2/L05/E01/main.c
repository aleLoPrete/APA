#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSTRING 256

typedef struct {
    int nOfS;
    char **songs;
}friend;

friend *getSongs(int *friends);
int ruleOfProd(int pos, friend *val, char **sol, int n, int count);
void printSol(char **sol, int dimSol, int count);

int main() {
    int numOfFriends, i;

    friend *songsCollection = getSongs(&numOfFriends);

    char **sol = (char **)malloc(numOfFriends * sizeof(char *));
    for (i = 0; i < numOfFriends; i++)
        sol[i] = malloc(MAXSTRING * sizeof(char));

    printf("Numero di playlist possibili: %d", ruleOfProd(0, songsCollection, sol, numOfFriends, 0));

    free(songsCollection);
    for (i = 0; i < numOfFriends; i++)
        free(sol[i]);
    free(sol);

    return 0;
}

friend *getSongs(int *theNumOfFriends) {
    FILE *fPnt;
    int j=0, i, k;

    fPnt = fopen("brani.txt", "r");

    fscanf(fPnt, "%d", theNumOfFriends);

    //ALLOCO DINAMICAMENTE LA MATRICE 'friends'
    friend *friends = (friend *)malloc((*theNumOfFriends) * sizeof(friend));
    for (k = 0; k < *theNumOfFriends; k++) {
        friends[k].songs = malloc((*theNumOfFriends) * sizeof(char *));
        for (i = 0; i < *theNumOfFriends; i++)
            friends[k].songs[i] = malloc(MAXSTRING * sizeof(char));
    }

    while (fscanf(fPnt, "%d", &friends[j].nOfS) != EOF) {
        for (i = 0; i < friends[j].nOfS; i++)
            fscanf(fPnt, "%s", friends[j].songs[i]);
        j++;
    }
    return friends;
}

int ruleOfProd(int pos, friend *val, char **sol, int dimSol, int count) {
    int i;
    if (pos >= dimSol) {
        printSol(sol,dimSol,count);
        return count + 1;
    }
    for (i = 0; i < val[pos].nOfS; i++) {
        strcpy(sol[pos], val[pos].songs[i]);
        count = ruleOfProd(pos + 1, val, sol, dimSol, count);
    }
    return count;
}

void printSol(char **sol, int dimSol, int count){
    printf("Playlist %d:\n", count + 1);
    for (int i = 0; i < dimSol; i++)
        printf("%s\n", sol[i]);
    printf("\n");
}