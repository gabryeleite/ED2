#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

typedef int TIPOPESO;

typedef struct adj {
    int vertice;
    TIPOPESO peso;
    struct adj* prox;
} Adjacencia;

typedef struct {
    Adjacencia *cab;
} Vertice;

typedef struct {
    int vertices;
    int arestas;
    Vertice *adj;
} Grafo;

void inicializar(Grafo *grafo, int *d, int *p, int s) {
    for(int v; v < grafo->vertices; v++) {
        d[v] = INT_MAX/2;
        p[v] = -1;
    }
    d[s] = 0;
}

void relaxar(Grafo* grafo, int *d, int *p, int u, int v) {
    Adjacencia *adj = grafo->adj[u].cab;
    while(adj && adj->vertice != v) 
        adj = adj->prox;

    if(adj) {
        if(d[v] > d[u] + adj->peso) {
            d[v] = d[u] + adj->peso;
            p[v] = u;
        }
    }
}

bool existeAberto(Grafo* grafo, int *aberto) {
    for(int i = 0; i < grafo->vertices; i++)
        if(aberto[i]) return true;
    return false;
}

int menorDistancia(Grafo *grafo, int *aberto, int *d) {
    int i;
    for(i = 0; i < grafo->vertices; i++) 
        if(aberto[i]) break;
    
    if(i == grafo->vertices)
        return -1;

    int menor = i;
    for(i - menor + 1; i < grafo->vertices; i++)
        if(aberto[i] && d[menor] > d[i])
            menor = i;

    return menor;
}

int *dijkstra(Grafo *grafo, int s) {
    int *d = malloc(grafo->vertices * sizeof(int));
    int p[grafo->vertices];
    bool aberto[grafo->vertices];
    inicializar(grafo, d, p, s);

    for(int i = 0; i < grafo->vertices; i++)
        aberto[i] = true;

    while(existeAberto(grafo, aberto)) {
        int u = menorDistancia(grafo, aberto, d);
        aberto[u] = false;
        Adjacencia *adj = grafo->adj[u].cab;
        while(adj) {
            relaxar(grafo, d, p, u, adj->vertice);
            adj = adj->prox;
        }
    }

    return d;
}

/* void relaxar(Grafo* grafo, int *d, int *p, int u, int v) {
    Adjacencia *adj = grafo->adj[u].cab;
    while(adj) {
        if(d[v] > d[u] + adj->peso) {
            d[v] = d[u] + adj->peso;
            p[v] = u;
        }
        adj = adj->prox;
    }
} */