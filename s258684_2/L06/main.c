#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct{
    int *nodes, *links;
}graph;

graph getGraph(FILE *theFp);
void printGraph(graph *theGraph);
void getVertexCover(graph *theGraph);
void computePwS(int pos, int **thePowerset, int *sol, int dimSol);

int main() {

    char PATHfile[30] = "grafo.txt";
    FILE *fp = fopen(PATHfile, "r");
    graph theGraph;

    if(fp==NULL)
        exit(99);
    theGraph = getGraph(fp);
    printf("vertex cover:\n");
    getVertexCover(&theGraph);

    return 0;
}

graph getGraph(FILE *theFp){
    int numNodes, numLinks, i, count=0;
    graph theGraph;

    fscanf(theFp, "%d %d", &numNodes, &numLinks);
    numLinks *=2;
    theGraph.links = (int *)malloc(numLinks*sizeof(int));
    theGraph.nodes = (int *)malloc(numNodes*sizeof(int));
    //inizializzo vettore dei nodi
    for(i = 0; i <numNodes; i++)
        theGraph.nodes[i] = -1;
    //acquisisco vettore dei vertici
    for(i = 0; i < numLinks; i += 2)
        fscanf(theFp,"%d %d", &theGraph.links[i], &theGraph.links[i+1]);

    //acquisisco vettore dei nodi dall'insieme dei vertici
    for(i = 0; i < numLinks; i++){ // in questo caso posso usare un'accesso diretto perché ho numeri interi di dimesione ridotta, altrimenti avrei dovuto accedere con costo lineare
        if(theGraph.nodes[theGraph.links[i]] == -1) {
            theGraph.nodes[theGraph.links[i]] = theGraph.links[i];
        }
    }

    return theGraph;
}

void printGraph(graph *theGraph){
    size_t numNodes = sizeof(theGraph->nodes);
    size_t numLinks = sizeof(theGraph->links)*2;

    printf("Nodes set:\n");
    for(int i=0; i<numNodes; i++)
        printf("%d\n", theGraph->nodes[i]);
    printf("Links set:\n");
    for(int i=0; i < numLinks; i+=2)
        printf("%d %d\n", theGraph->links[i], theGraph->links[i+1]);
    return;
}

void getVertexCover(graph *theGraph) {
    int **powerset, dimPwS, sol, numLinks4Node=0;
    size_t numNodes = sizeof(theGraph->nodes);
    size_t numLinks = sizeof(theGraph->links);

    dimPwS = pow(2, numNodes);
    powerset = (int **) malloc(dimPwS*sizeof(int *));
    for(int i=0; i < dimPwS; i ++)
        powerset[i] = (int *)malloc(numNodes*sizeof(int));

    sol = (int *)malloc(numLinks*sizeof(int));

    for(int i=0; i<dimPwS; i++){
        for(int j=0; j<numNodes; j++){
            powerset[i][j]= -1;
        }
    }

    computePwS(0, powerset, sol, numNodes);

    for(int i=0; i<dimPwS; i++){//scorro tutto lo spazio delle soluzioni
        numLinks4Node=0;
        for(int j=0; j<numNodes; j++){//per ogni elemento della soluzione conto in quanti vertici è presente
            if(powerset[i][j]==1){
                for(int k=0; k<numLinks*2; k++) {
                    if(j == theGraph->links[k]){
                        numLinks4Node++;
                    }
                }
            }
        }
        if(numLinks4Node>numLinks) {
            printf("> ");
            for(int f=0; f<numNodes;f++){
                if(powerset[i][f]!=0){
                    printf("%d ", theGraph->nodes[f]);
                }
            }
            printf("\n");
        }
    }

}

void computePwS(int pos, int **thePowerset, int *sol, int dimSol){
    int k;
    if(pos >= dimSol){
        k=0;
        while(thePowerset[k][0]!=-1){
            k++;
        }
        for(int i=0; i<dimSol; i++){
            thePowerset[k][i]=sol[i];
        }
        return;
    }
    sol[pos] = 0;
    computePwS(pos+1, thePowerset, sol, dimSol);
    sol[pos] = 1;
    computePwS(pos+1, thePowerset, sol, dimSol);
}