#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

#define WHITE 0
#define GRAY 1
#define BLACK 2
#define NIL -1

int tempo = 0; 

void visitaDFS(int u, int V, int A[][V], int cor[], int pai[], int d[], int f[]) {
    cor[u] = GRAY;
    d[u] = ++tempo;
    printf("%d ", u); // Ação específica para o DFS, como imprimir ou armazenar o vértice

    // para cada vertice v adjacente a u
    for(int i = 0; i < A[u][0]; i++) {
        int v = A[u][i + 1];
        if(cor[v] == WHITE) {
            pai[v] = u;
            visitaDFS(v, V, A, cor, pai, d, f);
        }
    }
    cor[u] = BLACK;
    f[u] = ++tempo;
}

void DFS(int V, int A[][V]) {
    int cor[V], pai[V];
    int d[V], f[V]; // d[]: momento cinza(descoberto), f[]: momento preto(examinou vizinhos) 
    // inicializa vertices
    for(int u = 0; u < V; u++) {
        cor[u] = WHITE;
        pai[u] = NIL;
    }

    // para cada vertice u
    for(int u = 0; u < V; u++) {
        if(cor[u] == WHITE) 
            visitaDFS(u, V, A, cor, pai, d, f);
    }
    // o algoritmo pode ser implmentado sem a necessidade da variavel tempo e os vetores d[] e f[]
}
