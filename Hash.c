#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// 15 -> 2 * 15 = 30 -> 31
#define TAM 31

typedef struct {
    int id;
    char descricao[200];
    float preco;
} Produto;

typedef struct aux {
    struct aux *prox;
    Produto produto;
} No;

typedef struct {
    No* inicio;
} Lista;

Produto cadastrarProduto(int id, char descricao[], float preco) {
    Produto novo;
    novo.id = id;
    strcpy(novo.descricao, descricao);
    novo.preco = preco;

    return novo;
}

// TABELA HASH COM ENCADEAMENTO SEPARADO
// utilizando método da divisão

void inicializarLista(Lista* l) {
    l->inicio = NULL;
}

void inserirLista(Lista* l, Produto produto) {
    No* novo = malloc(sizeof(No));
    // verificar alocação
    novo->produto = produto;
    novo->prox = l->inicio;
    l->inicio = novo;
}

int buscarLista(Lista* l, int id) {
    No *aux = l->inicio;
    while(aux && aux->produto.id != id) 
        aux = aux->prox;

    if(aux)
        return aux->produto.id;
    else
        return -1;
}

void imprimirLista(Lista* l) {
    No* aux = l->inicio;
    while(aux) {
        printf("%2d ", aux->produto.id);
        aux = aux->prox;
    }
}

void inicializarSeparado(Lista hash[]) {
    for(int i=0; i< TAM; i++)
        inicializarLista(&hash[i]);
    // define vazio como NULL
}

int espalharSeparado(int id) {
    return id % TAM;
}

void inserirSeparado(Lista hash[], Produto produto) {
    int index = espalharSeparado(produto.id);
    inserirLista(&hash[index], produto);
}

int buscarSeparado(Lista hash[], int id) {
    int index = espalharSeparado(id);
    return buscarLista(&hash[index], id);
}

void imprimirSeparado(Lista hash[]) {
    for(int i=0; i < TAM; i++) {
        printf("[%2d] = ", i);
        imprimirLista(&hash[i]);
        printf("\n");
    }
}

void removerSeparado(Lista hash[], int id) {
    int index = espalharSeparado(id);
    No* atual = hash[index].inicio;
    No* anterior = NULL;

    while(atual != NULL && atual->produto.id != id) {
        anterior = atual;
        atual = atual->prox;
    }

    if(atual == NULL) {
        printf("Produto com id %d nao encontrado.\n", id);
        return;
    }

    if(anterior == NULL) // primeiro da lista
        hash[index].inicio = atual->prox;
    else 
        anterior->prox = atual->prox;

    free(atual);
}

void destruirLista(Lista* l) {
    No* atual = l->inicio;
    No* proximo = NULL;

    while(atual) {
        proximo = atual->prox;
        free(atual);
        atual = proximo;
    }
    l->inicio = NULL; 
}

void destruirSeparado(Lista hash[]) {
    for(int i = 0; i < TAM; i++) 
        destruirLista(&hash[i]);
}

// TABELA HASH COM ENCADEAMENTO ABERTO
// utilizando método da dobra e duplo hash

void inicializarAberto(Produto hash[]) {
    for(int i=0; i< TAM; i++)
        hash[i].id = -1;
    // defino -1 como "vazio"
}

char soma(char a, char b) {
    int soma = (a - '0') + (b - '0'); 
    return (soma % 10) + '0'; // retorna dígito da unidade
}

int converte(char *str) {
    int num = 0;
    for(int i = 0; str[i] != '\0'; ++i) {
        num = num * 10 + str[i] - '0';
    }
    return num;
}

int espalharAberto(int id) {
    int i, t = 2; // tamanho dobra
    char idStr[6]; // no maximo 6
    char temp[6];
    sprintf(idStr, "%d", id);
    int tam = strlen(idStr); // tam = digitos
    strcpy(temp, idStr);

    for(i=0; i < (tam/t) - 1; i++) {
        for(int j = 0 ; j < t ; ++j ) { 
            temp[i*t + j+t] = soma(temp[i*t+j+t], temp[(i*t + j+t)-(2*j + 1)]);
        }
    }
    return converte(&temp[i * t]);
}

int passoHash(int id) {
    return 1 + (id % (TAM - 1));
}

void inserirAberto(Produto hash[], Produto produto) {
    int index = espalharAberto(produto.id);
    int passo = passoHash(produto.id);

    while(hash[index].id != -1) {
        index = (index + passo) % TAM;
    }
    hash[index] = produto;
}

int buscarAberto(Produto hash[], int id) {
    int index = espalharAberto(id);
    int passo = passoHash(id);

    while(hash[index].id != -1) {
        if(hash[index].id == id)
            return index;
        index = (index + passo) % TAM;
    }
    return -1;
}

void imprimirAberto(Produto hash[]) {
    for(int i=0; i < TAM; i++) 
        printf("[%2d] = %2d\n", i, hash[i].id);
}

void removerAberto(Produto hash[], int id) {
    int index = buscarAberto(hash, id);
    if(index == -1)
        return;
    hash[index].id = -1;
}

// nao precisa destruirAberto()

int main() {
    Lista hashSeparado[TAM];
    Produto hashAberto[TAM];
    Produto produto;

    inicializarSeparado(hashSeparado);
    inicializarAberto(hashAberto);

    // inserçao produtos tabela hash
    produto = cadastrarProduto(5, "Produto 1", 10.99);
    inserirSeparado(hashSeparado, produto);
    inserirAberto(hashAberto, produto);

    produto = cadastrarProduto(15, "Produto 2", 20.49);
    inserirSeparado(hashSeparado, produto);
    inserirAberto(hashAberto, produto);

    produto = cadastrarProduto(25, "Produto 3", 30.79);
    inserirSeparado(hashSeparado, produto);
    inserirAberto(hashAberto, produto);

    produto = cadastrarProduto(30, "Produto 4", 50.89);
    inserirSeparado(hashSeparado, produto);
    inserirAberto(hashAberto, produto);

    produto = cadastrarProduto(35, "Produto 5", 15.49);
    inserirSeparado(hashSeparado, produto);
    inserirAberto(hashAberto, produto);

    produto = cadastrarProduto(40, "Produto 6", 70.69);
    inserirSeparado(hashSeparado, produto);
    inserirAberto(hashAberto, produto);

    produto = cadastrarProduto(4, "Produto 7", 90.99);
    inserirSeparado(hashSeparado, produto);
    inserirAberto(hashAberto, produto);

    // imprimir tabela hash
    printf("Tabela Hash com Encadeamento Separado:\n");
    imprimirSeparado(hashSeparado);
    printf("\nTabela Hash com Encadeamento Aberto:\n");
    imprimirAberto(hashAberto);

    // remover um produto
    int idRemover = 35;
    removerSeparado(hashSeparado, idRemover);
    removerAberto(hashAberto, idRemover);

    printf("\nRemoção produto com id %d\n", idRemover);

    // imprimir tabela hash após remoção
    printf("\nTabela Hash com Encadeamento Separado:\n");
    imprimirSeparado(hashSeparado);
    printf("\nTabela Hash com Encadeamento Aberto:\n");
    imprimirAberto(hashAberto);

    destruirSeparado(hashSeparado);

    return 0;
}
