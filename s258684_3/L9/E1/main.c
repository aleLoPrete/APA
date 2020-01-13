#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int start;
    int end;
} activity;

activity *openFile(int *numOfActivities);

int partition(activity *theActivities, int l, int r);

void swap(activity *theActivities, int a, int b);

void qkSort(activity *theActivities, int numOfActivities);

void qkSortExt(activity *theActivities, int l, int r);

void findMCA(activity *theActivities, int numOfActivities);

void printMCA(activity *theActivities, int *P, int i);

int main() {
    int numOfActivities;
    activity *activities;

    activities= openFile(&numOfActivities);
    qkSort(activities, numOfActivities);

    findMCA(activities, numOfActivities);
    free(activities);
}

activity *openFile(int *numOfActivities) {
    FILE *fp;
    int i = 0;
    activity *activities;

    fp = fopen("att.txt", "r");
    if (fp == NULL) {
        printf("File att.txt non trovato");
        exit(99);
    }

    fscanf(fp, "%d", numOfActivities);

    activities = malloc(*numOfActivities * sizeof(activity));
    while (fscanf(fp, "%d %d", &activities[i].start, &activities[i].end) != EOF)
        i++;

    return activities;
}

int partition(activity *theActivities, int l, int r) {
    int i = l - 1, j = r;
    activity x = theActivities[r];
    for (;;) {
        while (theActivities[++i].end < x.end);
        while (theActivities[--j].end > x.end)
            if (j == l)
                break;
        if (i >= j)
            break;
        swap(theActivities, i, j);
    }
    swap(theActivities, i, r);
    return i;
}

void swap(activity *theActivities, int a, int b) {
    activity tmp;
    tmp = theActivities[a];
    theActivities[a] = theActivities[b];
    theActivities[b] = tmp;
}

void qkSort(activity *theActivities, int numOfActivities) {
    int l = 0, r = numOfActivities - 1;
    qkSortExt(theActivities, l, r);
}

void qkSortExt(activity *theActivities, int l, int r) {
    int q;
    if (r <= l)
        return;
    q = partition(theActivities, l, r);
    qkSortExt(theActivities, l, q - 1);
    qkSortExt(theActivities, q + 1, r);
}

void findMCA(activity *theActivities, int numOfActivities) {
    int i, j, *L, *P, activityCounter = 1, last = 1;
    L = malloc(numOfActivities * sizeof(int));
    P = malloc(numOfActivities * sizeof(int));

    for (i = 0; i < numOfActivities; i++) {
        L[i] = 0;
        P[i] = -1;
        for (j = 0; j < i; j++)
            if (theActivities[i].start >= theActivities[j].end && L[i] < L[j] + (theActivities[j].end - theActivities[j].start)) {
                L[i] += theActivities[j].end - theActivities[j].start;
                P[i] = j;
            }
        if (activityCounter < L[i]) {
            activityCounter = L[i];
            last = i;
        }
    }
    printf("La sequenza di attivita' compatibili che massimizza la somma delle durate e':\n");
    printMCA(theActivities, P, last);
    printf("\nNumero di attivita': %d", activityCounter);
    free(L);
    free(P);
}

void printMCA(activity *theActivities, int *P, int i) {
    if (P[i] == -1) {
        printf("(%d , %d)\t", theActivities[i].start, theActivities[i].end);
        return;
    }
    printMCA(theActivities, P, P[i]);
    printf("(%d , %d)\t", theActivities[i].start, theActivities[i].end);
}
