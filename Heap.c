#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int pid;
    int prioridade;
    int tempoEspera;
} Heap, *Processo;

Heap adicionarProcesso(int* pid, int prioridade, int tempoEspera) {
    Heap processo;
    processo.pid = *pid;
    processo.prioridade = prioridade;
    processo.tempoEspera = tempoEspera;
    (*pid)++;

    return processo;
}

void troca(Processo a, Processo b) {
    Heap temp = *a;
    *a = *b;
    *b = temp;
}

int heapVazio(int n) {
    // retorna 1 se o heap estiver vazio, caso contrário retorna 0
    return (n <= 0) ? 1 : 0;
}

// (Processo heap = vetor; int i = indice)
void subirMax(Processo heap, int i) {
    int pai = i / 2;

    // Prioridade
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

    // Tempo de Espera
    if(pai >= 1) {
        if(heap[i].tempoEspera < heap[pai].tempoEspera) {
            // faz a subida
            troca(&heap[i], &heap[pai]);
            subirMin(heap, pai);
        }
    }

}

// (Processo heap = vetor; int i = indice; int n = tamanho)
void descerMax(Processo heap, int i, int n) {
    int maior = i;
    int esq = 2 * i;
    int dir = 2 * i + 1;

    // Prioridade
    if(esq <= n && heap[esq].prioridade > heap[maior].prioridade)
        maior = esq;

    if(dir <= n && heap[dir].prioridade > heap[maior].prioridade)
        maior = dir;

    if(maior != i) {
        troca(&heap[i], &heap[maior]);
        descerMax(heap, maior, n);
    }

}

// (Processo heap = vetor; int i = indice; int n = tamanho)
void descerMin(Processo heap, int i, int n) {
    int menor = i;
    int esq = 2 * i;
    int dir = 2 * i + 1;

    // Tempo de Espera
    if(esq <= n && heap[esq].tempoEspera < heap[menor].tempoEspera)
        menor = esq;

    if(dir <= n && heap[dir].tempoEspera < heap[menor].tempoEspera)
        menor = dir;

    if(menor != i) {
        troca(&heap[i], &heap[menor]);
        descerMin(heap, menor, n);
    }

}

// (Processo heap = vetor; int prioridade; int tempo; int n = tamanho; int tipo = 1:max ou 0:min)
void inserirProcesso(Processo* heap, Heap processo, int* tam, int tipo) {
    // aumenta tamanho do vetor
    *heap = (Processo)realloc(*heap, sizeof(Heap) * (*tam + 2));
    (*tam)++;
    (*heap)[*tam] = processo;

    if(tipo == 1)
        subirMax(*heap, *tam);
    else    
        subirMin(*heap, *tam);

}

// (Processo heap = vetor; int n = tamanho; int tipo = 1:prioridade ou 0:tempo de espera)
void imprimirProcesso(Processo heapMax, Processo heapMin, int n, int tipo) {
    if(tipo == 1) {
        printf("\nPrioridade:\nHeapMax:\n");
        for(int i = 1; i <= n; i++) 
            printf("%d ", heapMax[i].prioridade);
        printf("\nHeapMin:\n");
        for(int i = 1; i <= n; i++) 
            printf("%d ", heapMin[i].prioridade);
    } else {
        printf("\nTempo de Espera:\nHeapMax:\n");
        for(int i = 1; i <= n; i++) 
            printf("%d ", heapMax[i].tempoEspera);
        printf("\nHeapMin:\n");
        for(int i = 1; i <= n; i++) 
            printf("%d ", heapMin[i].tempoEspera);
    }
    printf("\n");
}

// (Processo heap = vetor; int n = tamanho; int tipo = 1:max ou 0:min)
void removerProcessoTopo(Processo *heap, int *tam, int tipo) {
    if(!heapVazio(*tam)) {
        (*heap)[1] = (*heap)[*tam];
        (*tam)--;
        // diminui tamanho do vetor
        *heap = (Processo)realloc(*heap, sizeof(Heap) * (*tam + 1));

        if(tipo == 1)
            descerMax(*heap, 1, *tam);
        else    
            descerMin(*heap, 1, *tam);
    }

}

void construirHeapMax(Processo heap, int n) {
    int j = n / 2;
    for(int i = j; i >= 1; i--)
        descerMax(heap, i, n);

}

void construirHeapMin(Processo heap, int n) {
    int j = n / 2;
    for(int i = j; i >= 1; i--)
        descerMin(heap, i, n);
        
}

void criarDotPrioridade(Processo heap, const char* nome, int n) {
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

void criarDotTempoEspera(Processo heap, const char* nome, int n) {
    FILE* fdot = fopen(nome, "w");
    if(fdot == NULL) 
        return;

    fprintf(fdot, "digraph Heap {\n");
    fprintf(fdot, "\tnode [shape=circle];\n");

    for(int i = 1; i <= n; i++) {
        int esq = 2 * i;
        int dir = 2 * i + 1;

        if(esq <= n) 
            fprintf(fdot, "\t\"%d\" -> \"%d\"\n", heap[i].tempoEspera, heap[esq].tempoEspera);

        if(dir <= n) 
            fprintf(fdot, "\t\"%d\" -> \"%d\"\n", heap[i].tempoEspera, heap[dir].tempoEspera);

    }
    fprintf(fdot, "}\n");
    fclose(fdot);
}

int main() {
    int tamMax = 0, tamMin = 0, id = 1;
    Heap processo;
    Processo heapMax = (Processo)malloc(sizeof(Heap));
    Processo heapMin = (Processo)malloc(sizeof(Heap));
    
    processo = adicionarProcesso(&id, 10, 7);
    inserirProcesso(&heapMax, processo, &tamMax, 1);
    inserirProcesso(&heapMin, processo, &tamMin, 0);

    processo = adicionarProcesso(&id, 20, 6);
    inserirProcesso(&heapMax, processo, &tamMax, 1);
    inserirProcesso(&heapMin, processo, &tamMin, 0);

    processo = adicionarProcesso(&id, 30, 8);
    inserirProcesso(&heapMax, processo, &tamMax, 1);
    inserirProcesso(&heapMin, processo, &tamMin, 0);

    processo = adicionarProcesso(&id, 60, 4);
    inserirProcesso(&heapMax, processo, &tamMax, 1);
    inserirProcesso(&heapMin, processo, &tamMin, 0);

    processo = adicionarProcesso(&id, 50, 2);
    inserirProcesso(&heapMax, processo, &tamMax, 1);
    inserirProcesso(&heapMin, processo, &tamMin, 0);

    processo = adicionarProcesso(&id, 40, 3);
    inserirProcesso(&heapMax, processo, &tamMax, 1);
    inserirProcesso(&heapMin, processo, &tamMin, 0);

    removerProcessoTopo(&heapMax, &tamMax, 1);
    removerProcessoTopo(&heapMin, &tamMin, 0);

    //removerProcessoTopo(&heapMax, &tamMax, 1);
    //removerProcessoTopo(&heapMin, &tamMin, 0);

    imprimirProcesso(heapMax, heapMin, tamMax, 1);
    imprimirProcesso(heapMax, heapMin, tamMax, 0);

    criarDotPrioridade(heapMax, "prioridade.dot", tamMax);
    criarDotTempoEspera(heapMin, "tempoEspera.dot", tamMin);

    return 0;
}
