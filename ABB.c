#include <stdio.h>
#include <stdlib.h>

typedef struct no {
    int chave;
    struct no *esq;
    struct no *dir;
} No;

No* CriarNo(int chave) {
    No *novo = malloc(sizeof(No));
    // verificar alocação
    novo->chave = chave;
    novo->esq = NULL;
    novo->dir = NULL;
    
    return novo;
}

No* inserirNo(No *raiz, int chave) {
    // verifica se a raiz é nula
    if(raiz == NULL) 
        return CriarNo(chave);

    // verifica se já existe
    if(chave == raiz->chave) {
        printf("Chave %d ja se encontra na arvore!\n", chave);
        return raiz;
    }

    // inserção na esquerda
    if(chave < raiz->chave) 
        raiz->esq = inserirNo(raiz->esq, chave);
    // inserção na direita
    else 
        raiz->dir = inserirNo(raiz->dir, chave);

    return raiz;
}

void preOrdem(No *raiz) {
    // RED 
    if(raiz){ 
        printf("%d ", raiz->chave);
        preOrdem(raiz->esq);
        preOrdem(raiz->dir);
    }
}

void ordemSimetrica(No *raiz) {
    // ERD
    if(raiz){
        ordemSimetrica(raiz->esq);
        printf("%d ", raiz->chave);
        ordemSimetrica(raiz->dir);
    }
}

No* buscarNo(No* raiz, int chave) {
    // caso base: raiz nula ou chave encontrada
    if(raiz == NULL || raiz->chave == chave)
        return raiz;

    // busca pela esquerda
    if(chave < raiz->chave) 
        return buscarNo(raiz->esq, chave);
    // busca pela direita
    else
        return buscarNo(raiz->dir, chave);
}

No* menorDireita(No* raiz) {
    // menor nó à direita é o nó mais à esquerda
    No* atual = raiz;
    while(atual->esq != NULL){
        atual = atual->esq;
    }
    return atual;
}

No* removerNo(No* raiz, int chave) {
    // caso base: raiz nula
    if(raiz == NULL)
        return raiz;

    // localizar o nó a ser removido
    // busca pela esquerda
    if(chave < raiz->chave)
        raiz->esq = removerNo(raiz->esq, chave);
    // busca pela direita
    else if(chave > raiz->chave)
        raiz->dir = removerNo(raiz->dir, chave);
    // nó encontrado
    else{
        // caso 1: nó folha ou nó com 1 filho (esquerda e/ou direita == NULL)
        if(raiz->esq == NULL){
            No *aux = raiz->dir; // guarda valor do nó à direita (mesmo que seja nulo)
            free(raiz);
            return aux; // substitui a raiz pelo seu nó à direita
        }else if(raiz->dir == NULL){
            No *aux = raiz->esq; // guarda valor do nó à esquerda
            free(raiz);
            return aux; // substitui a raiz pelo seu nó à esquerda
        }

        // caso 2: nó com 2 filhos
        // substituir pelo maior nó à esquerda ou menor à direita
        No *aux = menorDireita(raiz->dir);
        // substituição, troca o nó atual pelo menor à direita e o exclui
        raiz->chave = aux->chave; 
        raiz->dir = removerNo(raiz->dir, aux->chave);
    }

    return raiz;
}

int main() {
    // inicializa árvore
    No* raiz = NULL;

    // inserção de elementos
    raiz = inserirNo(raiz, 42);
    raiz = inserirNo(raiz, 24);
    raiz = inserirNo(raiz, 98);
    raiz = inserirNo(raiz, 1);
    raiz = inserirNo(raiz, 13);
    raiz = inserirNo(raiz, 87);
    raiz = inserirNo(raiz, 66);

    // impressão ordem simétrica (ordem crescente)
    printf("Ordem Simetrica:\n");
    ordemSimetrica(raiz);
    printf("\n");

    // impressão pre ordem (RED)
    printf("\nPre Ordem:\n");
    preOrdem(raiz);
    printf("\n");

    // remove nó 42
    raiz = removerNo(raiz, 42);

    // impressão pós remoção nó 42
    printf("\nOrdem Simetrica:\n");
    ordemSimetrica(raiz);
    printf("\n");

    // libera memória alocada para a árvore (remove todos os nós)
    while(raiz != NULL){
        raiz = removerNo(raiz, raiz->chave);
    }

    return 0;
}
