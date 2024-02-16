#include <stdio.h>
#include <stdlib.h>

typedef int TIPOCHAVE;

typedef struct aux {
    TIPOCHAVE chave;
    struct aux* pai; 
    struct aux* esq; 
    struct aux* dir;
    int cor; // 0: preto | 1: vermelho
} NO, *ALUNO;

typedef struct {
    ALUNO raiz; // identifica determinado nó como raiz
} RAIZ;

void inicializarRB(RAIZ* T) {
    T->raiz = NULL;
}

ALUNO criarNovoAluno(TIPOCHAVE ch) {
    ALUNO novoNo = (ALUNO)malloc(sizeof(NO));
    novoNo->chave = ch;
    novoNo->pai = NULL;
    novoNo->esq = NULL;
    novoNo->dir = NULL;
    novoNo->cor = 1; // vermelho por padrão
    return novoNo;
}

void rotacaoL(RAIZ* T, ALUNO x) {
    ALUNO y = x->dir;
    x->dir = y->esq;
    if(y->esq != NULL)
        y->esq->pai = x;
    y->pai = x->pai;
    if(x->pai == NULL)
        T->raiz = y;
    else if(x == x->pai->esq)
        x->pai->esq = y;
    else
        x->pai->dir = y;
    y->esq = x;
    x->pai = y;
}

void rotacaoR(RAIZ* T, ALUNO y) {
    ALUNO x = y->esq;
    y->esq = x->dir;
    if(x->dir != NULL)
        x->dir->pai = y;
    x->pai = y->pai;
    if(y->pai == NULL)
        T->raiz = x;
    else if(y == y->pai->dir)
        y->pai->dir = x;
    else
        y->pai->esq = x;
    x->dir = y;
    y->pai = x;
}

void inserirAlunoFixup(RAIZ* T, ALUNO z) {
    while(z->pai != NULL && z->pai->cor == 1) {
        if(z->pai == z->pai->pai->esq) {
            ALUNO y = z->pai->pai->dir;
            // CASO 1
            if(y != NULL && y->cor == 1) {
                z->pai->cor = 0;
                y->cor = 0;
                z->pai->pai->cor = 1;
                z = z->pai->pai;
            } else {
            // CASO 2
                if(z == z->pai->dir) { // || y->cor != 1
                    z = z->pai;
                    rotacaoL(T, z);
                }
            // CASO 3
                z->pai->cor = 0;
                z->pai->pai->cor = 1;
                rotacaoR(T, z->pai->pai);
            }
        // Caso espelho da cláusula anterior
        } else if((z->pai == z->pai->pai->dir)) {
            ALUNO y = z->pai->pai->esq;
            // CASO 1
            if(y != NULL && y->cor == 1) {
                z->pai->cor = 0;
                y->cor = 0;
                z->pai->pai->cor = 1;
                z = z->pai->pai;
            } else {
            // CASO 2
                if(z == z->pai->esq) { // || y->cor != 1
                    z = z->pai;
                    rotacaoR(T, z);
                }
            // CASO 3
                z->pai->cor = 0;
                z->pai->pai->cor = 1;
                rotacaoL(T, z->pai->pai);
            }
        }
    }
    T->raiz->cor = 0;
}

void inserirAluno(RAIZ* T, TIPOCHAVE ch) {
    ALUNO z = criarNovoAluno(ch);
    ALUNO y = NULL;
    ALUNO x = T->raiz;
    while(x != NULL) {
        y = x;
        if(z->chave < x->chave)
            x = x->esq;
        else
            x = x->dir;
    }
    z->pai = y;
    if(y == NULL)
        T->raiz = z;
    else if(z->chave < y->chave)
        y->esq = z;
    else
        y->dir = z;
    z->esq = NULL;
    z->dir = NULL;
    z->cor = 1; // Vermelho
    inserirAlunoFixup(T, z);
}

void exibirArvoreEmOrdem(ALUNO raiz) {
    if(raiz != NULL) {
        exibirArvoreEmOrdem(raiz->esq);
        printf("%d ", raiz->chave);
        exibirArvoreEmOrdem(raiz->dir);
    }
}

ALUNO buscarAluno(ALUNO x, TIPOCHAVE ch, int *custo) {
    if(x == NULL || x->chave == ch) {
        (*custo)++;
        return x;
    }
    (*custo)++;
    if(ch < x->chave)
        return buscarAluno(x->esq, ch, custo);
    return buscarAluno(x->dir, ch, custo);
}

void removerAlunoFixup(RAIZ* T, ALUNO x) {
    while(x != T->raiz && x->cor == 0) {
        if(x == x->pai->esq) {
            ALUNO w = x->pai->dir;
            // CASO 1
            if(w->cor == 1) {
                w->cor = 0;
                x->pai->cor = 1;
                rotacaoL(T, x->pai);
                w = x->pai->dir;
            }
            // CASO 2
            if(w->esq->cor == 0 &&  w->dir->cor == 0) {
                w->cor = 1;
                x = x->pai;
                //x->pai = x->pai->pai;
            } else {
            // CASO 3
                if(w->dir->cor == 0) {
                    w->esq->cor = 0;
                    w->cor = 1;
                    rotacaoR(T, w);
                    w = x->pai->dir;
                }
            // CASO 4
                w->cor = x->pai->cor;
                x->pai->cor = 0;
                w->dir->cor = 0;
                rotacaoL(T, x->pai);
                x = T->raiz;
            }
        
        } else {
        // Caso espelho da cláusula anterior
            ALUNO w = x->pai->esq; 
            // CASO 1
            if(w->cor == 1) {
                w->cor = 0;
                x->pai->cor = 1;
                rotacaoR(T, x->pai);
                w = x->pai->esq;
            }
            // CASO 2
            if(w->dir->cor == 0 &&  w->esq->cor == 0) {
                w->cor = 1;
                x = x->pai;
                //x->pai = x->pai->pai;
            } else {
            // CASO 3
                if(w->esq->cor == 0) {
                    w->dir->cor = 0;
                    w->cor = 1;
                    rotacaoL(T, w);
                    w = x->pai->esq;
                }
            // CASO 4
                w->cor = x->pai->cor;
                x->pai->cor = 0;
                w->esq->cor = 0;
                rotacaoR(T, x->pai);
                x = T->raiz;
            }
        }

    }
    x->cor = 0;
    while(T->raiz->pai != NULL)
        T->raiz = T->raiz->pai;
}

ALUNO minimo(ALUNO x) {
    while(x->esq != NULL)
        x = x->esq;
    return x;
}

ALUNO sucessor(ALUNO x) {
    if(x->dir != NULL)
        return minimo(x->dir);

    ALUNO y = x->pai;
    while(y != NULL && x == y->dir) {
        x = y;
        y = y->pai;
    }
    return y;
}


void removerAluno(RAIZ* T, TIPOCHAVE ch) {
    int custo = 0;
    ALUNO z = buscarAluno(T->raiz, ch, &custo);
    if(z == NULL)
        return;

    ALUNO y, x;
    if(z->esq == NULL || z->dir == NULL)
        y = z; // Caso 1
    else
        y = sucessor(z); // Caso 2

    if(y->esq != NULL)
        x = y->esq;
    else
        x = y->dir;

    if(x != NULL)
        x->pai = y->pai; // Faça isso, mesmo se x for NULL
    
    if(y->pai == NULL)
        T->raiz = x;
    else if (y == y->pai->esq)
        y->pai->esq = x;
    else
        y->pai->dir = x;

    if(y != z)
        z->chave = y->chave;

    if(y->cor == 0 && x != NULL)
        removerAlunoFixup(T, x);

    free(y);
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int alturaTotal(ALUNO x) {
    if(x == NULL) 
        return -1; // nó nulo tem altura -1
    
    int alturaEsq = alturaTotal(x->esq);
    int alturaDir = alturaTotal(x->dir);
    
    return 1 + max(alturaEsq, alturaDir);
}

void destruirAux(ALUNO x) {
    if(x != NULL){
        destruirAux(x->esq); 
        destruirAux(x->dir); 

        free(x); 
    }
}

void destruirArvoreRB(RAIZ* T) {
    // libera todos os nós da árvore
    destruirAux(T->raiz); 

    // define raiz como NULL
    T->raiz = NULL; 
}

void alturaNegraAux(ALUNO x, int altAtual, int *altNegra) {
    if(x == NULL) {
        if(*altNegra == -1 || altAtual < *altNegra)
            *altNegra = altAtual;
        return;
    }
    
    if(x->cor == 0) // Nó preto
        altAtual++;
    
    alturaNegraAux(x->esq, altAtual, altNegra);
    alturaNegraAux(x->dir, altAtual, altNegra);
}

int alturaNegra(RAIZ *T) {
    if(T == NULL || T->raiz == NULL) 
        return -1;
    
    int altNegra = -1;
    alturaNegraAux(T->raiz, 0, &altNegra);

    return altNegra;
}

void contarNos(ALUNO x, int *rubro, int *preto) {
    if(x == NULL)
        return;

    if(x->cor == 1)
        (*rubro)++;
    else
        (*preto)++;

    contarNos(x->esq, rubro, preto);
    contarNos(x->dir, rubro, preto);
}

void calcularPorcentagem(ALUNO x) {
    if(x == NULL)
        return;

    int rubro = 0;
    int negro = 0;

    contarNos(x, &rubro, &negro);

    int total = rubro + negro;

    float percentRubro = (float)rubro / total * 100;
    float percentNegro = (float)negro / total * 100;

    printf("\nRubros: %.2f%%\nNegros: %.2f%%\n", percentRubro, percentNegro);
}

void ligarNo(RAIZ* T, char* nome, FILE* fdot) {
    if(T->raiz == NULL) 
        return;
    ALUNO arvore = T->raiz;

    fdot = fopen(nome, "a+");
    fprintf(fdot, "\n\t\t\"%d\\ncor:%d\"", arvore->chave, arvore->cor);

    if(arvore->esq != NULL) {
        fprintf(fdot, "\n\t\t\"%d\\ncor:%d\" -> \"%d\\ncor:%d\"\n", arvore->chave, arvore->cor, arvore->esq->chave, arvore->esq->cor);
        if(arvore->esq->cor == 0) // cor = 0 é preto
            fprintf(fdot, "\t\t\"%d\\ncor:%d\"  [style=filled, fillcolor=black, fontcolor=white];\n", arvore->esq->chave, arvore->esq->cor);
        else // cor = 1 é vermelho
            fprintf(fdot, "\t\t\"%d\\ncor:%d\"  [style=filled, fillcolor=red, fontcolor=black];\n", arvore->esq->chave, arvore->esq->cor);

        RAIZ aux;
        aux.raiz = arvore->esq;

        ligarNo(&aux, nome, fdot);
    }

    if(arvore->dir != NULL) {
        fprintf(fdot, "\n\t\t\"%d\\ncor:%d\" -> \"%d\\ncor:%d\"\n", arvore->chave, arvore->cor, arvore->dir->chave, arvore->dir->cor);
        if(arvore->dir->cor == 0) // cor = 0 é preto
            fprintf(fdot, "\t\t\"%d\\ncor:%d\"  [style=filled, fillcolor=black, fontcolor=white];\n", arvore->dir->chave, arvore->dir->cor);
        else // cor = 1 é vermelho
            fprintf(fdot, "\t\t\"%d\\ncor:%d\"  [style=filled, fillcolor=red, fontcolor=black];\n", arvore->dir->chave, arvore->dir->cor);

        RAIZ aux;
        aux.raiz = arvore->dir;

        ligarNo(&aux, nome, fdot);
    }

    fclose(fdot);
}

FILE* inicializarDot(char* fn) {
    FILE* fdot = fopen(fn, "w");

    fprintf(fdot, "\tdigraph BlackRedTree {\n");
    fprintf(fdot, "\tnode [shape=circle];\n");
    fclose(fdot);
    return fdot;
}

void terminarDot(char* fn) {
    FILE* fdot = fopen(fn,"a+");

    fprintf(fdot,"\n\t}");
    fclose(fdot);
}

int main() {
    RAIZ arvore;
    inicializarRB(&arvore);

    inserirAluno(&arvore, 10);
    inserirAluno(&arvore, 20);
    inserirAluno(&arvore, 30);
    inserirAluno(&arvore, 40);
    inserirAluno(&arvore, 50);
    inserirAluno(&arvore, 9);
    inserirAluno(&arvore, 8);
    inserirAluno(&arvore, 6);
    inserirAluno(&arvore, 7);
    inserirAluno(&arvore, 5);
    inserirAluno(&arvore, 60);

    removerAluno(&arvore, 20);
    removerAluno(&arvore, 9);
    removerAluno(&arvore, 50);
    removerAluno(&arvore, 30); 

    printf("Arvore Rubro-Negra em ordem:\n");
    exibirArvoreEmOrdem(arvore.raiz);
    printf("\n");

    int custo = 0;
    ALUNO busca = buscarAluno(arvore.raiz, 60, &custo);
    printf(busca != NULL ? "\nAluno encontrado!\nComparacoes: %d\n" : "\nAluno nao encontrado!\nComparacoes: %d\n", custo);

    printf("\nAltura total da arvore: %d\n", alturaTotal(arvore.raiz));
    printf("Altura Negra da arvore: %d\n", alturaNegra(&arvore));

    calcularPorcentagem(arvore.raiz);

    FILE* ArqDot = inicializarDot("arvore.dot");
    ligarNo(&arvore, "arvore.dot", ArqDot);
    terminarDot("arvore.dot");

    // libera memória alocada para a árvore (remove todos os nós)
    destruirArvoreRB(&arvore);

    return 0;
}
