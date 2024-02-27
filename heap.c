#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int pid;
    int prioridade;
    int tempoEspera;
} Heap, *Processo;

void troca(Processo a, Processo b) {
    Heap temp = *a;
    *a = *b;
    *b = temp;
}

// (Processo heap = vetor; int i = indice)
void subirMax(Processo heap, int i) {
    int pai = i / 2;

    if(pai >= 1) {
        if(heap[i].prioridade > heap[pai].prioridade) {
            // faz a subida
            troca(&heap[i], &heap[pai]);
            subirMax(heap, pai);
        }
    }

}

// (Processo heap = vetor; int i = indice)
void subirMin(Processo heap, int i) {
    int pai = i / 2;

    if(pai >= 1) {
        if(heap[i].prioridade < heap[pai].prioridade) {
            // faz a subida
            troca(&heap[i], &heap[pai]);
            subirMin(heap, pai);
        }
    }

}

// (Processo heap = vetor; int i = indice; int n = tamanho)
void descerMax(Processo heap, int i, int n) {
    int maior = i;
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;

    if(esq <= n && heap[esq].prioridade > heap[maior].prioridade)
        maior = esq;

    if(dir <= n && heap[dir].prioridade < heap[maior].prioridade)
        maior = dir;

    if(maior != i) {
        troca(&heap[i], &heap[maior]);
        descerMax(heap, maior, n);
    }

}

// (Processo heap = vetor; int i = indice; int n = tamanho)
void descerMin(Processo heap, int i, int n) {
    int menor = i;
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;

    if(esq <= n && heap[esq].prioridade < heap[menor].prioridade)
        menor = esq;

    if(dir <= n && heap[dir].prioridade < heap[menor].prioridade)
        menor = dir;

    if(menor != i) {
        troca(&heap[i], &heap[menor]);
        descerMin(heap, menor, n);
    }

}

int inserirMax(Processo heap, int prioridade, int tempo, int n) {
    // aumenta tamanho do vetor
    heap = (Processo)realloc(heap, sizeof(Heap) * (n + 2));
    n++;
    heap[n].prioridade = prioridade;
    heap[n].tempoEspera = tempo;

    subirMax(heap, n);

    // retorna tamanho atualizado
    return n;
}

int inserirMin(Processo heap, int prioridade, int tempo, int n) {
    // aumenta tamanho do vetor
    heap = (Processo)realloc(heap, sizeof(Heap) * (n + 2));
    n++;
    heap[n].prioridade = prioridade;
    heap[n].tempoEspera = tempo;

    subirMin(heap, n);

    // retorna tamanho atualizado
    return n;
}

void imprimirHeap(Processo heap, int n) {
    for (int i = 1; i <= n; i++) {
        printf("%d ", heap[i].prioridade);
    }
    printf("\n");
}

int excluirMin(Processo heap, int n) {
    heap[1] = heap[n];
    n--;
    // diminui tamanho do vetor
    heap = (Processo)realloc(heap, sizeof(Heap) * (n + 1));

    descerMin(heap, 1, n);

    // retorna tamanho atualizado
    return n;
}

int excluirMax(Processo heap, int n) {
    heap[1] = heap[n];
    n--;
    // diminui tamanho do vetor
    heap = (Processo)realloc(heap, sizeof(Heap) * (n + 1));

    descerMax(heap, 1, n);

    // retorna tamanho atualizado
    return n;
}

void criarArquivoDot(Processo heap, const char* nome, int n) {
    FILE* fdot = fopen(nome, "w");
    if(fdot == NULL) 
        return;

    fprintf(fdot, "digraph Heap {\n");
    fprintf(fdot, "\tnode [shape=circle];\n");

    for(int i = 1; i <= n; i++) {
        int esq = 2 * i;
        int dir = 2 * i + 1;

        if(esq <= n) 
            fprintf(fdot, "\t\"%d\" -> \"%d\"\n", heap[i].prioridade, heap[esq].prioridade);

        if(dir <= n) 
            fprintf(fdot, "\t\"%d\" -> \"%d\"\n", heap[i].prioridade, heap[dir].prioridade);

    }

    fprintf(fdot, "}\n");
    fclose(fdot);
}


int main() {
    Processo heap = (Processo)malloc(sizeof(Heap));
    int n = 0;

    n = inserirMax(heap, 10, 1, n);
    n = inserirMax(heap, 20, 1, n);
    n = inserirMax(heap, 30, 1, n);
    n = inserirMax(heap, 60, 1, n);
    n = inserirMax(heap, 50, 1, n);
    n = inserirMax(heap, 40, 1, n);

    printf("Heap:\n");
    imprimirHeap(heap, n);

    criarArquivoDot(heap, "heap.dot", n);

    return 0;
}
