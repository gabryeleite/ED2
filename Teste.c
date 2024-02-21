#include <stdio.h>
#include <stdlib.h>

typedef int TIPOCHAVE;

typedef struct aux {
    TIPOCHAVE id;
    struct aux* pai; 
    struct aux* esq; 
    struct aux* dir;
    int cor; // 0: preto | 1: vermelho
} NO, *ALUNO;

typedef struct {
    ALUNO raiz; // identifica determinado nó como raiz
    ALUNO nil;  // identifica determinado nó como nulo
} RAIZ;

void inicializarRB(RAIZ* T) {
    T->nil = (ALUNO)malloc(sizeof(NO));
    T->nil->cor = 0; // Nulo é preto: 0
    T->raiz = T->nil;
}

ALUNO criarNovoAluno(RAIZ* T, TIPOCHAVE ID) {
    ALUNO novo = (ALUNO)malloc(sizeof(NO));
    novo->id = ID;
    novo->pai = T->nil;
    novo->esq = T->nil;
    novo->dir = T->nil;
    novo->cor = 1; // vermelho por padrão
    return novo;
}

void rotacaoL(RAIZ* T, ALUNO x) {
    ALUNO y = x->dir;
    x->dir = y->esq;
    if(y->esq != T->nil)
        y->esq->pai = x;
    y->pai = x->pai;
    if(x->pai == T->nil)
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
    if(x->dir != T->nil)
        x->dir->pai = y;
    x->pai = y->pai;
    if(y->pai == T->nil)
        T->raiz = x;
    else if(y == y->pai->dir)
        y->pai->dir = x;
    else
        y->pai->esq = x;
    x->dir = y;
    y->pai = x;
}

void inserirAlunoFixup(RAIZ* T, ALUNO z) {
    while(z->pai != T->nil && z->pai->cor == 1) {
        if(z->pai == z->pai->pai->esq) {
            ALUNO y = z->pai->pai->dir;
            // CASO 1
            if(y != T->nil && y->cor == 1) {
                printf("CASO 1: Pai fica negro; Irmão fica negro; Avô fica rubro.\n");
                z->pai->cor = 0;
                y->cor = 0;
                z->pai->pai->cor = 1;
                z = z->pai->pai;
            } else {
            // CASO 2
                if(z == z->pai->dir) { // || y->cor != 1
                    printf("CASO 2: Rotaciona o pai a esquerda.\n");
                    z = z->pai;
                    rotacaoL(T, z);
                }
            // CASO 3
                printf("CASO 3: Pai fica negro; Avô fica rubro; Rotaciona o avô a direita.\n");
                z->pai->cor = 0;
                z->pai->pai->cor = 1;
                rotacaoR(T, z->pai->pai);
            }
        // Caso espelho da cláusula anterior
        } else if((z->pai == z->pai->pai->dir)) {
            ALUNO y = z->pai->pai->esq;
            // CASO 1
            if(y != T->nil && y->cor == 1) {
                printf("CASO 1: Pai fica negro; Irmão fica negro; Avô fica rubro.\n");
                z->pai->cor = 0;
                y->cor = 0;
                z->pai->pai->cor = 1;
                z = z->pai->pai;
            } else {
            // CASO 2
                if(z == z->pai->esq) { // || y->cor != 1
                printf("CASO 2: Rotaciona o pai a direita.\n");
                    z = z->pai;
                    rotacaoR(T, z);
                }
            // CASO 3
                printf("CASO 3: Pai fica negro; Avô fica rubro; Rotaciona o avô a esquerda.\n");
                z->pai->cor = 0;
                z->pai->pai->cor = 1;
                rotacaoL(T, z->pai->pai);
            }
        }
    }
    T->raiz->cor = 0;
}

void inserirAluno(RAIZ* T, TIPOCHAVE ID) {
    ALUNO z = criarNovoAluno(T, ID);
    ALUNO y = T->nil;
    ALUNO x = T->raiz;
    while(x != T->nil) {
        y = x;
        if(z->id < x->id)
            x = x->esq;
        else
            x = x->dir;
    }
    z->pai = y;
    if(y == T->nil)
        T->raiz = z;
    else if(z->id < y->id)
        y->esq = z;
    else
        y->dir = z;
    z->esq = T->nil;
    z->dir = T->nil;
    z->cor = 1; // Vermelho
    inserirAlunoFixup(T, z);
}

void exibirArvoreEmOrdem(RAIZ* T, ALUNO x) {
    if(x != T->nil) {
        exibirArvoreEmOrdem(T, x->esq);
        printf("%04d ", x->id);
        exibirArvoreEmOrdem(T, x->dir);
    }
}

ALUNO buscarAluno(RAIZ* T, ALUNO x, TIPOCHAVE ID, int *custo) {
    if(x == T->nil || x->id == ID) {
        (*custo)++;
        return x;
    }
    (*custo)++;
    if(ID < x->id)
        return buscarAluno(T, x->esq, ID, custo);
    return buscarAluno(T, x->dir, ID, custo);
}

void removerAlunoFixup(RAIZ* T, ALUNO x) {
    while(x != T->raiz && x->cor == 0) {
        if(x == x->pai->esq) {
            ALUNO w = x->pai->dir;
            // CASO 1
            if(w->cor == 1) {
                printf("CASO 1: Irmão fica negro; Pai fica rubro; Rotaciona o pai a esquerda.\n");
                w->cor = 0;
                x->pai->cor = 1;
                rotacaoL(T, x->pai);
                w = x->pai->dir;
            }
            // CASO 2
            if(w->esq->cor == 0 &&  w->dir->cor == 0) {
                printf("CASO 2: Irmão fica rubro; Suba um nível na árvore.\n");
                w->cor = 1;
                x = x->pai;
                //x->pai = x->pai->pai;
            } else {
            // CASO 3
                if(w->dir->cor == 0) {
                    printf("CASO 3: Irmão fica rubro; Sobrinho esquerdo fica negro; Rotaciona o irmão a direita.\n");
                    w->esq->cor = 0;
                    w->cor = 1;
                    rotacaoR(T, w);
                    w = x->pai->dir;
                }
            // CASO 4
                printf("CASO 4: Pai fica negro; Irmão fica rubro; Sobrinho direito fica negro; Rotaciona o pai a esquerda.\n");
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
                printf("CASO 1: Irmão fica negro; Pai fica rubro; Rotaciona o pai a direita.\n");
                w->cor = 0;
                x->pai->cor = 1;
                rotacaoR(T, x->pai);
                w = x->pai->esq;
            }
            // CASO 2
            if(w->dir->cor == 0 &&  w->esq->cor == 0) {
                printf("CASO 2: Irmão fica rubro; Suba um nível na árvore.\n");
                w->cor = 1;
                x = x->pai;
                //x->pai = x->pai->pai;
            } else {
            // CASO 3
                if(w->esq->cor == 0) {
                    printf("CASO 3: Irmão fica rubro; Sobrinho direito fica negro; Rotaciona o irmão a esquerda.\n");
                    w->dir->cor = 0;
                    w->cor = 1;
                    rotacaoL(T, w);
                    w = x->pai->esq;
                }
            // CASO 4
                printf("CASO 4: Pai fica negro; Irmão fica rubro; Sobrinho esquerdo fica negro; Rotaciona o pai a direita.\n");
                w->cor = x->pai->cor;
                x->pai->cor = 0;
                w->esq->cor = 0;
                rotacaoR(T, x->pai);
                x = T->raiz;
            }
        }

    }
    x->cor = 0;

    while(T->raiz->pai != T->nil)
        T->raiz = T->raiz->pai;
}

ALUNO minimo(RAIZ* T, ALUNO x) {
    while(x->esq != T->nil)
        x = x->esq;
    return x;
}

// AQUi E NO FINAL DE INSERIR
ALUNO sucessor(RAIZ* T, ALUNO x) {
    if(x->dir != T->nil)
        return minimo(T, x->dir);

    ALUNO y = x->pai;
    while(y != T->nil && x == y->dir) {
        x = y;
        y = y->pai;
    }
    return y;
}


void removerAluno(RAIZ* T, TIPOCHAVE ID) {
    int custo = 0;
    ALUNO z = buscarAluno(T, T->raiz, ID, &custo);
    if(z == T->nil)
        return;

    ALUNO y, x;
    if(z->esq == T->nil || z->dir == T->nil)
        y = z; // Caso 1
    else
        y = sucessor(T, z); // Caso 2

    if(y->esq != T->nil)
        x = y->esq;
    else
        x = y->dir;

    if(x != T->nil)
        x->pai = y->pai; // Faça isso, mesmo se x for NULL
    
    if(y->pai == T->nil)
        T->raiz = x;
    else if (y == y->pai->esq)
        y->pai->esq = x;
    else
        y->pai->dir = x;

    if(y != z)
        z->id = y->id;

    if(y->cor == 0 && x != T->nil) {
        removerAlunoFixup(T, x);
    }

    free(y);
} 

int max(int a, int b) {
    return (a > b) ? a : b;
}

int alturaTotal(RAIZ* T, ALUNO x) {
    if(x == T->nil) 
        return -1; // nó nulo tem altura -1
    
    int alturaEsq = alturaTotal(T, x->esq);
    int alturaDir = alturaTotal(T, x->dir);
    
    return 1 + max(alturaEsq, alturaDir);
}

void destruirAux(RAIZ* T, ALUNO x) {
    if(x != T->nil){
        destruirAux(T, x->esq); 
        destruirAux(T, x->dir); 

        free(x); 
    }
}

void destruirArvoreRB(RAIZ* T) {
    // libera todos os nós da árvore
    destruirAux(T, T->raiz); 

    // define raiz como NULL
    T->raiz = T->nil; 
}

void alturaNegraAux(RAIZ* T, ALUNO x, int altAtual, int *altNegra) {
    if(x == T->nil) {
        if(*altNegra == -1 || altAtual < *altNegra)
            *altNegra = altAtual;
        return;
    }
    
    if(x->cor == 0) // Nó preto
        altAtual++;
    
    alturaNegraAux(T, x->esq, altAtual, altNegra);
    alturaNegraAux(T, x->dir, altAtual, altNegra);
}

int alturaNegra(RAIZ *T) {
    if(T == NULL || T->raiz == T->nil) 
        return -1;
    
    int altNegra = -1;
    alturaNegraAux(T, T->raiz, 0, &altNegra);

    return altNegra;
}

void contarNos(RAIZ *T, ALUNO x, int *rubro, int *preto) {
    if(x == T->nil)
        return;

    if(x->cor == 1)
        (*rubro)++;
    else
        (*preto)++;

    contarNos(T, x->esq, rubro, preto);
    contarNos(T, x->dir, rubro, preto);
}

void calcularPorcentagem(RAIZ *T, ALUNO x) {
    if(x == T->nil)
        return;

    int rubro = 0;
    int negro = 0;

    contarNos(T, x, &rubro, &negro);

    int total = rubro + negro;

    float percentRubro = (float)rubro / total * 100;
    float percentNegro = (float)negro / total * 100;

    printf("\nRubros: %.2f%%\nNegros: %.2f%%\n", percentRubro, percentNegro);
}

void ligarNo(RAIZ* T, RAIZ* z, FILE* fdot) {
    if(z->raiz == T->nil) 
        return;
    ALUNO arvore = z->raiz;

    if(arvore->cor == 0) // cor = 0 é preto
        fprintf(fdot, "\t\t\"%04d\"  [style=filled, fillcolor=black, fontcolor=white];\n", arvore->id);
    else // cor = 1 é vermelho
        fprintf(fdot, "\t\t\"%04d\"  [style=filled, fillcolor=red, fontcolor=white];\n", arvore->id);

    if(arvore->esq != T->nil) {
        fprintf(fdot, "\n\t\t\"%04d\" -> \"%04d\"\n", arvore->id, arvore->esq->id);
        if(arvore->esq->cor == 0) // cor = 0 é preto
            fprintf(fdot, "\t\t\"%04d\"\n", arvore->esq->id);
        else // cor = 1 é vermelho
            fprintf(fdot, "\t\t\"%04d\"\n", arvore->esq->id);

        RAIZ aux;
        aux.raiz = arvore->esq;

        ligarNo(T, &aux, fdot);
    }

    if(arvore->dir != T->nil) {
        fprintf(fdot, "\n\t\t\"%04d\" -> \"%04d\"\n", arvore->id, arvore->dir->id);
        if(arvore->dir->cor == 0) // cor = 0 é preto
            fprintf(fdot, "\t\t\"%04d\"\n", arvore->dir->id);
        else // cor = 1 é vermelho
            fprintf(fdot, "\t\t\"%04d\"\n", arvore->dir->id);

        RAIZ aux;
        aux.raiz = arvore->dir;

        ligarNo(T, &aux, fdot);
    }

}

FILE* inicializarDot(char* fn) {
    FILE* fdot = fopen(fn, "w");

    fprintf(fdot, "\tdigraph RedBlackTree {\n");
    fprintf(fdot, "\tnode [shape=circle];\n");
    return fdot;
}

void terminarDot(FILE* fdot) {
    fprintf(fdot,"\n\t}");
    fclose(fdot);
} 

int main() {
    RAIZ T;
    inicializarRB(&T);

    //for(int i=0; i <= 200; i++) 
    //    inserirAluno(&T, i);  

    //insere de tras p frente
    for(int i=200; i >= 0; i--) 
        inserirAluno(&T, i); 

    printf("\nREMOCAO\n");

    for(int i=0; i <= 187; i++)
        removerAluno(&T, i); 
 
    // remove de tras p frente
    //for(int i=187; i >= 0; i--) 
    //    removerAluno(&T, i);   

    printf("Arvore Rubro-Negra em ordem:\n");
    exibirArvoreEmOrdem(&T, T.raiz);
    printf("\n");

    //int custo = 0;
    //ALUNO busca = buscarAluno(arvore.raiz, 60, &custo); (escolher valor para buscar)
    //printf(busca != NULL ? "\nAluno encontrado!\nComparacoes: %d\n" : "\nAluno nao encontrado!\nComparacoes: %d\n", custo);

    printf("\nAltura total da arvore: %d\n", alturaTotal(&T, T.raiz));
    printf("Altura Negra da arvore: %d\n", alturaNegra(&T));

    calcularPorcentagem(&T, T.raiz);

    FILE* fdot = inicializarDot("arvore.dot");
    ligarNo(&T, &T, fdot);
    terminarDot(fdot);

    // libera memória alocada para a árvore (remove todos os nós)
    destruirArvoreRB(&T);

    return 0;
}
