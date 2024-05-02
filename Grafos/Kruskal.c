#include <stdio.h> 
#define MAX 100

typedef struct { 
    int u, v, peso; 
} Aresta; 

int cmp(const void *a, const void *b) { 
    return ((Aresta *)a)->peso - ((Aresta *)b)->peso; 
} 

void kruskal(Aresta E[], int nAresta, int nVertice, Aresta A[]) { 
    int C[MAX]; // conjunto de componentes conexos
    int k, count = 0; 

    // ordena as arestas por peso
    qsort(E, nAresta, sizeof(Aresta), cmp); 

    // Inicializa o conjunto de componentes conexos
    for(int i = 0; i < nVertice; i++) 
        C[i] = i; 

    // para cada aresta (u,v) em E, em ordem de peso
    for(int i = 0; i < nAresta; i++) { 
        // se os vértices 'u' e 'v' não estão na mesma componente conexa
        if(C[E[i].u] != C[E[i].v]) { 
            A[count++] = E[i]; // adiciona a aresta (u,v) à árvore geradora mínima A
            printf("%d %d\n", E[i].u, E[i].v); 
            k = C[E[i].u]; // armazena o identificador da componente conexa de 'u'
            // para cada vértice em C
            for(int j = 0; j < nVertice; j++) {
                // se o vértice pertence à mesma componente conexa que 'u'
                if(C[j] == k) 
                    C[j] = C[E[i].v]; // atualiza o identificador da componente conexa para o de 'v'
            }
        } 
    } 
}

/* Kruskal(G(V,E,w),s): A // parametros: representaçao de grafo (V,E,w)
    A = vazio // A representa a árvore geradora mínima
    C = vazio // C: vetor que indica a qual árvore pertence cada vértice
    para i = {1,...,n} em V faça
        C[i] = i // make-set(i): marca cada vertice pertencente a um conjunto (árvore) distinto
    fim-para
    ordene as arestas de E por peso
    para cada aresta (u,v) em E, em ordem de peso, faça
        se C[u] != C[v] entao
            A = A uniao {(u,v)} // adiciona aresta(u,v) na arvore geradora minima
            k = C[u]
            para i = {1,...,n} em C faça
                se C[i] == k entao
                    C[i] = C[v]
                fim-se
            fim-para
        fim-se
    fim-para
    retorne A
FIM */
