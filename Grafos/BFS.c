#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

#define WHITE 0
#define GRAY 1
#define BLACK 2
#define NIL -1

void BFS(int V, int A[][V], int s) {
    int cor[V], dist[V], pai[V];
    // inicializa vertices
    for(int u = 0; u < V; u++) {
        cor[u] = WHITE;
        dist[u] = INT_MAX/2;
        pai[u] = NIL;
    }
    cor[s] = GRAY;
    dist[s] = 0;
    pai[s] = NIL;

    int Q[V], front = 0, rear = -1;
    Q[++rear] = s;

    // enquanto a fila Q nao estiver vazia
    while(front <= rear) {
        int u = Q[front++];\
        // para cada vertice v adjacente a u
        for(int i = 0; i < A[u][0]; i++) {
            int v = A[u][i + 1];
            if(cor[v] == WHITE) {
                cor[v] = GRAY;
                dist[v] = dist[u] + 1;
                pai[v] = u; // u vertece que precede v
                Q[++rear] = v;
            }
        }
        cor[u] = BLACK;
    }
}