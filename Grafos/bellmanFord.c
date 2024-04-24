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

void inicializarBF(Grafo *grafo, int *d, int *p, int s) {
    for(int v = 0; v < grafo->vertices; v++) {
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

int *bellmanFord(Grafo *grafo, int s) {
    int *d = malloc(grafo->vertices * sizeof(int));
    int p[grafo->vertices];
    inicializarBF(grafo, d, p, s);

    for(int i = 1; i <= grafo->vertices - 1; i++) {
        for(int u = 0; u < grafo->vertices; u++) {
            Adjacencia *adj = grafo->adj[u].cab;
            while(adj) {
                relaxar(grafo, d, p, u, adj->vertice);
                adj = adj->prox;
            }
        }
    }

    for(int u = 0; u < grafo->vertices; u++) {
        Adjacencia *adj = grafo->adj[u].cab;
        while(adj) {
            if(d[adj->vertice] > d[u] + adj->peso) { 
                free(d);
                return NULL;
            }
            adj = adj->prox;
        }
    }

    return d;
}

void adicionarAresta(Grafo *grafo, int u, int v, TIPOPESO peso) {
    Adjacencia *novo = malloc(sizeof(Adjacencia));
    novo->vertice = v;
    novo->peso = peso;
    novo->prox = grafo->adj[u].cab;
    grafo->adj[u].cab = novo;
}

void inicializarGrafo(Grafo* grafo, int n, int m) {
    grafo->vertices = n;
    grafo->arestas = m;
    grafo->adj = malloc(n * sizeof(Vertice));

    for(int i = 0; i < n; i++) 
        grafo->adj[i].cab = NULL;
}

void liberarGrafo(Grafo* grafo, int n) {
    for(int i = 0; i < n; i++) {
        Adjacencia *adj = grafo->adj[i].cab;
        while(adj) {
            Adjacencia *temp = adj;
            adj = adj->prox;
            free(temp);
        }
    }
    free(grafo->adj);
}

int main() {
    Grafo grafo;
    int n, m;

    printf("Número de cidades: ");
    scanf("%d", &n); // Nós
    printf("Número de rotas: ");
    scanf("%d", &m); // Arestas

    inicializarGrafo(&grafo, n, m);
    
    int u, v, custo, origem;
    printf("Rotas:\n");
    for(int i = 0; i < m; i++) {
        scanf("%d %d %d", &u, &v, &custo);
        adicionarAresta(&grafo, u, v, custo);
    }

    printf("Nó de origem: ");
    scanf("%d", &origem);

    int *distancias = bellmanFord(&grafo, origem);

    if(distancias) {
        printf("Menor caminho da cidade %d para demais cidades:\n", origem);
        for(int i = 0; i < n; i++) 
            printf("Cidade %d: %d\n", i, distancias[i]);
        free(distancias);
    } else 
        printf("Existe um ciclo de custo negativo alcançável a partir do nó de origem.\n");
    
    liberarGrafo(&grafo, n);

    return 0;
}