#include <stdio.h> 
#define MAX 1000 

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

/* KRUSKAL(G(V,E,w)) : A 
    A <- {}  // Inicializa a árvore geradora mínima como um conjunto vazio
    C <- {}  // Inicializa o conjunto de componentes conexos
    for i = {1,...,n} em V:  // Para cada vértice em V
        C[i] <- i  // Inicializa o conjunto de componentes conexos com vértices individuais

    ordene as arestas de E por peso  // Ordena as arestas em ordem crescente de peso
    for (u,v) in E, em ordem de peso:  // Para cada aresta (u,v) em E, em ordem de peso
        if C[u] != C[v]:  // Se os vértices 'u' e 'v' não estão na mesma componente conexa
            A <- A ∪ {(u,v)}  // Adiciona a aresta (u,v) à árvore geradora mínima A
            k <- C[u]  // Armazena o identificador da componente conexa de 'u'
            for i = {1,...,n} em C:  // Para cada vértice em C
                if C[i] = k:  // Se o vértice pertence à mesma componente conexa que 'u'
                    C[i] <- C[v]  // Atualiza o identificador da componente conexa para o de 'v'
                fim-se 
            fim-para 
        fim-se 
    return A  // Retorna a árvore geradora mínima A
FIM */