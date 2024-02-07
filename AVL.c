#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int TIPOCHAVE;

typedef struct aux {
    TIPOCHAVE chave;
    struct aux *esq;
    struct aux *dir;
    int bal;
} NO, *PONT;

/* Cria um novo (aloca memoria e preenche valores) no com chave=ch
e retorna seu endereco */
PONT criarNovoNo(TIPOCHAVE ch) {
    PONT novo = malloc(sizeof(NO));
    // verificar alocação
    novo->chave = ch;
    novo->esq = NULL;
    novo->dir = NULL;
    novo->bal = 0;
    
    return novo;
}

// Retorna o maior valor entre dois inteiros
int max(int a, int b) {
    // retorna maior altura entre dois nós
    return (a > b) ? a : b;
}

// Retorna a altura de uma (sub-)arvore
int altura(PONT p) {
    if(p == NULL)
        return -1; // nó nulo tem altura -1
  
    int alturaEsq = altura(p->esq);
    int alturaDir = altura(p->dir);

    return 1 + max(alturaEsq, alturaDir);
}

// Calcula fator de balanceamento do nó
int fatorBalanceamento(PONT raiz) {
    // nó nulo tem fator 0
    if(raiz == NULL)
        return 0;
    else
        return altura(raiz->dir) - altura(raiz->esq);
}

// Exibe arvore Em Ordem 
void exibirArvoreEmOrdem(PONT raiz) {
    // ERD
    if(raiz){
        exibirArvoreEmOrdem(raiz->esq);
        printf("%d ", raiz->chave);
        exibirArvoreEmOrdem(raiz->dir);
    }
}

// Exibe arvore Pre Ordem 
void exibirArvorePreOrdem(PONT raiz) {
    // RED 
    if(raiz){ 
        printf("%d ", raiz->chave);
        exibirArvorePreOrdem(raiz->esq);
        exibirArvorePreOrdem(raiz->dir);
    }
}

// Exibe arvore Pos Ordem 
void exibirArvorePosOrdem(PONT raiz) {
    // EDR 
    if(raiz){ 
        exibirArvorePosOrdem(raiz->esq);
        exibirArvorePosOrdem(raiz->dir);
        printf("%d ", raiz->chave);
    }
}

/* Rotações à direita (LL e LR) Retornará o endereço do nó que será a
nova raiz da subárvore originalmente iniciada por p */
PONT rotacaoL(PONT p) {
    // p é o nó com fator de balanceamento "-2" (pende à esquerda)
    PONT u = p->esq;
    
    // rotação simples esquerda (LL)
    if(u->bal == -1){
        p->esq = u->dir; // adota filho u
        u->dir = p; // u vira nova raiz e p seu filho direito
        p->bal = 0; // atualiza fator de balanceamento
        u->bal = 0;

        return u;
    }

    // rotação à esquerda e à direita (LR)
    if(u->bal == 1){
        PONT v = u->dir; 
        u->dir = v->esq;
        v->esq = u;
        p->esq = v->dir;
        v->dir = p;

        if(v->bal == -1) p->bal = -1;
        else p->bal = 0;

        if(v->bal == 1) u->bal = -1;
        else u->bal = 0;

        v->bal = 0;
        return v;
    }
    
}

/* Rotações à esquerda (RR e RL) Retornará o endereço do nó que será a
nova raiz da subárvore originalmente iniciada por p */
PONT rotacaoR(PONT p) {
    // p é o nó com fator de balanceamento "2" (pende à direita)
    PONT u = p->dir;
    
    // rotação simples esquerda (RR)
    if(u->bal == 1){
        p->dir = u->esq; // adota filho u
        u->esq = p; // u vira nova raiz e p seu filho esquerdo
        p->bal = 0; // atualiza fator de balanceamento
        u->bal = 0;

        return u;
    }

    // rotação à direita e à esquerda (LR)
    if(u->bal == -1){
        PONT v = u->esq; 
        u->esq = v->dir;
        v->dir = u;
        p->dir = v->esq;
        v->esq = p;

        if(v->bal == 1) p->bal = 1;
        else p->bal = 0;

        if(v->bal == -1) u->bal = 1;
        else u->bal = 0;

        v->bal = 0;
        return v;
    }
    
}

// Inserção AVL: p é inicializado com o endereco do nó raiz e *alterou com false
PONT inserirAVL(PONT p, TIPOCHAVE ch) {
    // cria nó
    if(p == NULL)
        return criarNovoNo(ch); 

    // insere na subárvore esquerda
    if(ch < p->chave) 
        p->esq = inserirAVL(p->esq, ch);
    // insere na subárvore direita
    else if(ch > p->chave)
        p->dir = inserirAVL(p->dir, ch);
    // chave já existe
    else 
        return p;

    // atualiza o fator de balanceamento
    p->bal = fatorBalanceamento(p);

    // verifica balanceamento
    // árvore pendendo à direita
    if (p->bal > 1)
        return rotacaoR(p);
    // árvore pendendo à esquerda
    else if (p->bal < -1)
        return rotacaoL(p);

    return p; 
}

/* Retorna o endereco do NO que contem chave=ch ou NULL caso a chave nao seja
encontrada. Utiliza busca binaria recursiva */
PONT buscaBinaria(TIPOCHAVE ch, PONT raiz) {
    // caso base: raiz nula ou chave encontrada
    if(raiz == NULL || raiz->chave == ch)
        return raiz;

    // busca pela esquerda
    if(ch < raiz->chave) 
        return buscaBinaria(ch, raiz->esq);
    // busca pela direita
    else
        return buscaBinaria(ch, raiz->dir);
}

// Busca binária não recursiva devolvendo o nó pai 
PONT buscaNo(PONT raiz, TIPOCHAVE ch, PONT *pai) {
    PONT atual = raiz;
    *pai = NULL; // nó pai nulo

    while(atual != NULL){
        // nó encontrado
        if(atual->chave == ch)
            return atual;
        
        *pai = atual; // atualiza nó pai

        // busca para esquerda
        if(ch < atual->chave)
            atual = atual->esq; 
        // busca para direita
        else 
            atual = atual->dir;
    }
    // retorna NULL se não encontrar 
    return NULL;
}

/* Auxilir da funcao excluirAVL, procura a menor chave à direita,
ou seja, a chave mais a esquerda */
PONT menorDireita(PONT raiz) {
    // menor nó à direita é o nó mais à esquerda
    PONT atual = raiz;
    while(atual->esq != NULL){
        atual = atual->esq;
    }
    return atual;
}

// Exclui a chave com valor igual a ch
PONT excluirAVL(PONT raiz, TIPOCHAVE ch) {
    // árvore vazia 
    if(raiz == NULL)
        return raiz; 
    
    // busca subárvore esquerda
    if(ch < raiz->chave) 
        raiz->esq = excluirAVL(raiz->esq, ch);
    // busca na subárvore direita
    else if(ch > raiz->chave) 
        raiz->dir = excluirAVL(raiz->dir, ch);
    // chave encontrada
    else{ 
        if(raiz->esq == NULL || raiz->dir == NULL) {
            // nó com um ou nenhum filho
            PONT temp;
            if(raiz->esq != NULL) temp = raiz->esq;
            else temp = raiz->dir;

            if(temp == NULL){ // nenhum filho
                temp = raiz;
                raiz = NULL;
            }else // Um filho
                *raiz = *temp; // salva conteúdo do filho

            free(temp);
        }else{
            // nó com dois filhos
            PONT temp = menorDireita(raiz->dir); // sucessor será menor chave à direita
            raiz->chave = temp->chave;
            raiz->dir = excluirAVL(raiz->dir, temp->chave);
        }
    }

    // se a árvore tinha apenas um nó, retorna NULL
    if(raiz == NULL)
        return raiz; 

    // atualiza o fator de balanceamento
    raiz->bal = fatorBalanceamento(raiz);

    // verifica balanceamento
    // árvore pendendo à direita
    if(raiz->bal > 1)
        return rotacaoR(raiz);
    // árvore pendendo à esquerda
    else if(raiz->bal < -1)
        return rotacaoL(raiz);

    return raiz; 
}

// funcao auxiliar na destruicao (liberacao da memoria) de uma arvore
void destruirAux(PONT subRaiz) {
    if (subRaiz != NULL) {
        // percorre nós filhos
        destruirAux(subRaiz->esq);
        destruirAux(subRaiz->dir);

        // libera nó atual
        free(subRaiz);
    }
}

// Libera toda memoria de uma arvore e coloca NULL no valor da raiz
void destruirArvore(PONT *raiz) {
    if(*raiz != NULL){
        destruirAux(*raiz);
        *raiz = NULL;
    }
}

// Inicializa arvore
void inicializar(PONT *raiz) {
    *raiz = NULL;
}

// retorna chave (usa para o dot)
TIPOCHAVE getChave(PONT p) {
    return p->chave;
}

// retorna fator de balanceamento (usa para o dot)
int getFb(PONT p) {
    return p->bal;
}

// Inicializa o arquivo .dot já aberto
void inicializarDot(FILE* fdot) {
    fprintf(fdot, "digraph Tree {\n");
    fprintf(fdot, "\tnode [shape=circle];\n");
    fflush(fdot);
}

// Termina e fecha o arquivo .dot
void terminarDot(FILE* fdot) {
    if(fdot != NULL){
        fprintf(fdot, "}");
        fclose(fdot);
    }

}

// Liga o Pai->Filho
void ligarNo(FILE* fdot, PONT pai, PONT filho) {
    if(filho == NULL)
        return;
    
    if(pai == NULL){
        TIPOCHAVE Chave = getChave(filho);
        int Fb = getFb(filho);
        fprintf(fdot, "\tRaiz -> \"%d\\nFb:%d\"\n", Chave, Fb);
    }else{
        TIPOCHAVE Chave1 = getChave(pai);
        TIPOCHAVE Chave2 = getChave(filho);
        int Fb1 = getFb(pai);
        int Fb2 = getFb(filho);
        fprintf(fdot, "    \"%d\\nFb:%d\" -> \"%d\\nFb:%d\"\n", Chave1, Fb1, Chave2, Fb2);
    }
    fflush(fdot);
}

// Cria o png a partir do .dot
void criarPngDot(const char nome[]) {
    char nomearq[strlen(nome) + 6];
    char nomepng[strlen(nome) + 6];

    int n = 1;
    sprintf(nomearq, "%s.dot", nome);
    sprintf(nomepng, "%s.png", nome);

    // Verifica se o arquivo já existe
    FILE *vrfy = fopen(nomearq, "r");
    while (vrfy != NULL)
    {
        fclose(vrfy);
        char command[2 * strlen(nomearq) + 30];
        sprintf(command, "dot -Tpng %s -o %s", nomearq, nomepng);

        system(command);

        n++;
        sprintf(nomearq, "%s-%d.dot", nome, n);
        sprintf(nomepng, "%s-%d.png", nome, n);
        vrfy = fopen(nomearq, "r");
    }

}

// percorre a arvore (usada na função imprimirAVL)
void percorrerAVL(FILE* ArqDot, PONT raiz, int nivel) {
    if(raiz){
        ligarNo(ArqDot, raiz, raiz->esq);
        ligarNo(ArqDot, raiz, raiz->dir);

        percorrerAVL(ArqDot, raiz->dir, nivel + 1);
        percorrerAVL(ArqDot, raiz->esq, nivel + 1);
    }

}

// gera dot da arvore AVL
void imprimirAVL(PONT raiz) {
    FILE *ArqDot = fopen("ArqDot.dot", "w");
    inicializarDot(ArqDot);
    ligarNo(ArqDot, NULL, raiz);

    percorrerAVL(ArqDot, raiz, 1);
    terminarDot(ArqDot);
}

int main() {
    
    PONT raiz;
    inicializar(&raiz);

    // inserção de elementos
    raiz = inserirAVL(raiz, 24);
    raiz = inserirAVL(raiz, 42);
    raiz = inserirAVL(raiz, 98);
    raiz = inserirAVL(raiz, 7);
    raiz = inserirAVL(raiz, 13);
    raiz = inserirAVL(raiz, 87);
    raiz = inserirAVL(raiz, 66);
    raiz = inserirAVL(raiz, 90);

    // desfaça o comentário para ver a exclusão
    // deixei dessa forma para ver os diferentes fb da árvore,
    // após a exclusão a árvore ficará cheia (todos fb = 0)
    //raiz = excluirAVL(raiz, 87);

    exibirArvoreEmOrdem(raiz);
    printf("\n");

    // impressação àrvore AVL
    imprimirAVL(raiz);
    criarPngDot("ArqDot");

    // libera memória alocada para a árvore (remove todos os nós)
    destruirArvore(&raiz);

    return 0;
}
