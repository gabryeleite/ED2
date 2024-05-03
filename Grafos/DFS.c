#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

#define WHITE 0
#define GRAY 1
#define BLACK 2
#define NIL -1

int tempo = 0; 

void visitaDFS(int u, int V, int A[][V], int cor[], int pai[], int d[]) {
    cor[u] = GRAY;
    tempo++;
    d[u] = tempo;
    printf("%d ", u); // Ação específica para o DFS, como imprimir ou armazenar o vértice

    // para cada vertice v adjacente a u
    for(int i = 0; i < A[u][0]; i++) {
        int v = A[u][i + 1];
        if(cor[v] == WHITE) {
            pai[v] = u;
            DFS_visit(v, V, A, cor, pai, d);
        }
    }
    cor[u] = BLACK;
}

void DFS(int V, int A[][V]) {
    int cor[V], pai[V], d[V];
    // inicializa vertices
    for(int u = 0; u < V; u++) {
        cor[u] = WHITE;
        pai[u] = NIL;
        d[u] = 0;
    }

    // para cada vertice u
    for(int u = 0; u < V; u++) {
        if(cor[u] == WHITE) {
            visitaDFS(u, V, A, cor, pai, d);
        }
    }
    // o algoritmo pode ser implmentado sem a necessidade da variavel tempo e o vetor d[]
}