#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int **matriz; 
    int vertice; 
    int aresta; 
} Grafo;

Grafo* inicializar(int n) {
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));

    grafo->vertice = n;
    grafo->aresta = 0;

    grafo->matriz = (int**)malloc(n * sizeof(int*));
    for(int i = 0; i < n; i++) {
        grafo->matriz[i] = (int*)calloc(n, sizeof(int));
    }

    return grafo;
}

void inserirAresta(Grafo* grafo, int u, int v, int peso) {
    u--;
    v--;
    // aresta de u para v com peso 
    if(u >= 0 && u < grafo->vertice && v >= 0 && v < grafo->vertice) {
        grafo->matriz[u][v] = peso;
        grafo->aresta++;
    } else 
        printf("Vertices invalidos!\n");
}

void removerAresta(Grafo* grafo, int u, int v) {
    u--;
    v--;
    if(u >= 0 && u < grafo->vertice && v >= 0 && v < grafo->vertice) {
        if(grafo->matriz[u][v] != 0) {
            grafo->matriz[u][v] = 0;
            grafo->aresta--;
        } else 
            printf("Aresta inexistente!\n");
    } else 
        printf("Vertices invalidos\n");
}

void imprimir(Grafo* grafo) {
    printf("\n    ");
    for(int j = 0; j < grafo->vertice; j++) {
        printf("v%d ", j + 1);
    }
    printf("\n");
    
    for(int i = 0; i < grafo->vertice; i++) {
        printf("v%d |", i + 1);
        for(int j = 0; j < grafo->vertice; j++) {
            printf("%2d ", grafo->matriz[i][j]);
        }
        printf("\n");
    }
}

int existeAresta(Grafo* grafo, int u, int v) {
    if(u >= 0 && u < grafo->vertice && v >= 0 && v < grafo->vertice) {
        return grafo->matriz[u][v] != 0; // retorna 1 se existe e 0 se nao
    } else 
        return 0; // vertice invalido
}

int numVertices(Grafo* grafo) {
    return grafo->vertice;
}

int numAresta(Grafo* grafo) {
    return grafo->aresta;
}

int existeVizinho(Grafo* grafo, int vertice) {
    if(vertice >= 0 && vertice < grafo->vertice) {
        for(int i = 0; i < grafo->vertice; i++) {
            if(grafo->matriz[vertice][i] != 0) 
                return 1; // existe
        }
        return 0;
    } else 
        return 0; // vertice invalido
    
}

int grauVertice(Grafo* grafo, int vertice) {
    vertice--;
    if(vertice >= 0 && vertice < grafo->vertice) {
        int grau = 0;
        for(int i = 0; i < grafo->vertice; i++) {
            // matriz[i][vertice] -> entrada, matriz[vertice][i] -> saida
            if(grafo->matriz[i][vertice] != 0 || grafo->matriz[vertice][i] != 0) 
                grau++;
        }
        return grau;
    } else 
        return -1; // vertice invalido 
}

void liberar(Grafo* grafo) {
    for(int i = 0; i < grafo->vertice; i++)
        free(grafo->matriz[i]);
    free(grafo->matriz);
    free(grafo);
}

void gerarDot(Grafo* grafo, const char* nome) {
    FILE *fdot;
    fdot = fopen(nome, "w");
    if(fdot == NULL) {
        printf("Erro ao abrir o arquivo!");
        return;
    }
    fprintf(fdot, "digraph Grafo {\n\tnode [shape=circle];\n");

    for(int i = 0; i < grafo->vertice; i++) {
        for(int j = 0; j < grafo->vertice; j++) {
            if(grafo->matriz[i][j] != 0)
                fprintf(fdot, "    v%d -> v%d [label=\"%d\"];\n", i+1, j+1, grafo->matriz[i][j]);
        }
    }
    fprintf(fdot, "}\n");

    fclose(fdot);
}

int main() {
    // numero vertices (tamanho matriz)
    Grafo* grafo = inicializar(5);

    // grafo, u, v, peso
    inserirAresta(grafo, 1, 2, 5);
    inserirAresta(grafo, 1, 3, 7);
    inserirAresta(grafo, 2, 3, 11);
    inserirAresta(grafo, 3, 5, 3);
    inserirAresta(grafo, 4, 5, 6);

    imprimir(grafo);

    printf("\nNumero vertices do Grafo: %d\n", numVertices(grafo));
    printf("Numero arestas do Grafo: %d\n", numAresta(grafo));
    printf("O grau do vertice 3 é %d\n", grauVertice(grafo, 3));

    //removerAresta(grafo, 1, 2);
    //imprimir(grafo); 
    gerarDot(grafo, "grafo.dot"); 

    liberar(grafo);

    return 0;
}
