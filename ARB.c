#include <stdio.h>
#include <stdlib.h>

typedef int TIPOCHAVE;

typedef struct aux {
    TIPOCHAVE id;
    struct aux* pai; // pont para o pai
    struct aux* esq; 
    struct aux* dir;
    int cor; // 0: preto | 1: vermelho
} NO, *ALUNO;

typedef struct {
    ALUNO raiz; // identifica determinado nó como raiz
} RAIZ;

ALUNO criarNovoAluno(TIPOCHAVE ID) {
    ALUNO novo = malloc(sizeof(NO));
    // verificar alocação
    novo->id = ID;
    novo->pai = NULL;
    novo->esq = NULL;
    novo->dir = NULL;
    novo->cor = 1; // nós são inicialmente vermelhos 

    return novo;
}

void rotacaoL(RAIZ* Arv, ALUNO p) {
    ALUNO filhoDir = p->dir;

    p->dir = filhoDir->esq;

    if(filhoDir->esq != NULL) 
        filhoDir->esq->pai = p;
    filhoDir->pai = p->pai;
    
    if(p->pai == NULL)
        Arv->raiz = filhoDir;
    else if(p == p->pai->esq)
        p->pai->esq = filhoDir;
    else 
        p->pai->dir = filhoDir;

    filhoDir->esq = p;
    p->pai = filhoDir;
}

void rotacaoR(RAIZ* Arv, ALUNO p) {
    ALUNO filhoEsq = p->esq;

    p->esq = filhoEsq->dir;

    if (filhoEsq->dir != NULL)
        filhoEsq->dir->pai = p;
    filhoEsq->pai = p->pai;

    if(p->pai == NULL) //caso seja a raiz 
        Arv->raiz = filhoEsq; 
    else if(p == p->pai->dir)
        p->pai->dir = filhoEsq;
    else 
        p->pai->esq = filhoEsq;
    
    filhoEsq->dir = p;
    p->pai = filhoEsq;
}

void balancearArv(RAIZ* Arv, ALUNO p) {
    // enquanto o pai for vermelho (propriedade 4)
    while (p != Arv->raiz && p->pai->cor == 1){ 
        // PONT Pai = p->pai;
        if(p->pai == p->pai->pai->esq){ // Pai à esquerda
            ALUNO tio = p->pai->pai->dir;
            // ================= CASO 1 ESQ ==================
            // se tio for vermelho
            if(tio != NULL && tio->cor == 1){ 
                p->pai->cor = 0; // Pai recebe preto
                tio->cor = 0; // tio recebe preto

                // se avô for raiz recebe preto
                if(p->pai->pai == Arv->raiz) 
                    p->pai->pai->cor = 0;
                // se nao, recebe vermelho
                else 
                    p->pai->pai->cor = 1;

                p = p->pai->pai;// p recebe avó para continuar busca

            }else{
                // ================= CASO 2 ESQ ==================
                // verifica se p está à direita
                if(p == p->pai->dir){ 
                    p = p->pai; // p recebe Pai
                    rotacaoL(Arv, p); // rotaciona p para à esquerda
                }
                // ================= CASO 3 ESQ ==================
                p->pai->cor = 0; // Pai recebe preto
                p->pai->pai->cor = 1; // avó recebe vermelho
                rotacaoR(Arv, p->pai->pai); // rotaciona avó à direita
            }
        // Mesmos casos porem o pai esta à direita
        }else{ 
            ALUNO tio = p->pai->pai->esq;
            // ================= CASO 1 DIR ==================
            if(tio != NULL && tio->cor == 1){
                p->pai->cor = 0;
                tio->cor = 0;
                p->pai->pai->cor = 1;

                p = p->pai->pai;
            }else{
                // ================= CASO 2 DIR ==================
                if(p == p->pai->esq){
                    p = p->pai;
                    rotacaoR(Arv, p); // rotaciona p à direita
                }
                // ================= CASO 3 DIR ==================
                p->pai->cor = 0;
                p->pai->pai->cor = 1;
                rotacaoL(Arv, p->pai->pai); // rotaciona avó à esquerda
            }
        }

    }
    Arv->raiz->cor = 0; // Define raiz como preta (Sempre)
}

void inserirAluno(RAIZ* Arv, TIPOCHAVE ID) {
    // Cria o novo nó que será inserido
    ALUNO novo = criarNovoAluno(ID);

    ALUNO pai = NULL; // declara nó pai que será usado nas comparações
    ALUNO atual = Arv->raiz; // "atual" é o ponteiro que percorrerá a árvore

    // busca local que será inserido o novo nó (igual ABB)
    while(atual != NULL) { // ponto de parada (chegou ao fim da árvore)
        pai = atual;
        if(ID < atual->id)
            atual = atual->esq;
        else if(ID > atual->id) 
            atual = atual->dir;
        else{
            printf("Chave %d ja se encontra na arvore!\n", ID);
            return;
        }

    }
    // atual agora é igual a NULL
    // pai é igual ao pai de atual
    novo->pai = pai;
    
    if(pai == NULL){ // árvore vazia
        Arv->raiz = novo; // raiz recebe o novo nó
        novo->cor = 0; // raiz é preta

    }else{ // se não, existe árvore
        if(ID < pai->id) 
            pai->esq = novo;
        else 
            pai->dir = novo;
    }
    // verifica as rotações e colorações necessárias para manter o balanceamento
    balancearArv(Arv, novo);
}

void exibirArvoreEmOrdem(ALUNO p) {
    if(p != NULL){
        exibirArvoreEmOrdem(p->esq);
        printf("%d ", p->id);
        exibirArvoreEmOrdem(p->dir);
    }
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int alturaTotal(ALUNO p) {
    if(p == NULL) 
        return -1; // nó nulo tem altura -1
    
    int alturaEsq = alturaTotal(p->esq);
    int alturaDir = alturaTotal(p->dir);
    
    return 1 + max(alturaEsq, alturaDir);
}

int buscarAluno(ALUNO raiz, TIPOCHAVE ID, int *custo) {
    // árvore vazia
    if(raiz == NULL)
        return 0; 

    int comparacoes = 0; // contador de comparações
    ALUNO atual = raiz;
    // percorre árvore
    while(atual != NULL){
        comparacoes++; // incrementa o contador

        // aluno encontrado
        if(atual->id == ID) {
            *custo = comparacoes;
            return 1; // retorna 1 se achou
        }
    
        // se menor, busca pela esquerda
        else if(ID < atual->id) 
            atual = atual->esq;
        // se maior, busca pela direita
        else 
            atual = atual->dir;
    
    }

    *custo = comparacoes;
    // retorna 0 se nao achou
    return 0;
}

void verificaNoPreto(RAIZ* Arv, RAIZ* pontArv, ALUNO removido, ALUNO paiRemovido) {
    ALUNO irmao;

    // Se o nó removido nao ser a raiz
    if((removido->pai != NULL)){
        // se removido for nó da esquerda
        if(removido == paiRemovido->esq){
            irmao = paiRemovido->dir; // irmao de removido (o que queremos deletar)

            // se o irmao for vermelho
            if(irmao->cor == 1){ 
                irmao->cor = 0; // irmao fica preto
                paiRemovido->cor = 1; // pai fica vermelho
                rotacaoL(pontArv, paiRemovido); // rotaciona 
                irmao = paiRemovido->dir; // irmao fica à direira do pai
            }
            // se não houver sobrinho à esquerda nem à direita ou se eles forem pretos
            if((irmao->esq == NULL || irmao->esq->cor == 0) && (irmao->dir == NULL || irmao->dir->cor == 0)){
                irmao->cor = 1; // muda cor do irmao (vermelho)
                removido = paiRemovido;
                paiRemovido = removido->pai; // paiRemovido recebe ávo de removido
            }else{
                // se o irmao for nulo ou preto
                if(irmao->dir == NULL || irmao->dir->cor == 0){
                    irmao->esq->cor = 0; // sobrinho esquerdo fica preto
                    irmao->cor = 1; // irmao fica vermelho
                    rotacaoR(pontArv, irmao); // rotaciona irmao para à direita
                    irmao = paiRemovido->dir; // irmao fica a direita do pai
                }

                irmao->cor = paiRemovido->cor; // irmao descende cor do pai
                paiRemovido->cor = 0; // pai fica preto
                irmao->dir->cor = 0; // sobrinho à direita fica preto
                rotacaoL(pontArv, paiRemovido); // pai rotaciona à esquerda
                removido = Arv->raiz; // removido vira raiz
            }

        // se removido for nó da direita
        }else{
            irmao = paiRemovido->esq; // irmao é nó da esquerda

            // se irmao for vermelho
            if(irmao->cor == 1){
                irmao->cor = 0; // irmao muda para preto
                paiRemovido->cor = 1; // pai fica vermelho
                rotacaoR(pontArv, paiRemovido); // rotaciona pai à direita
                irmao = paiRemovido->esq; // atualiza irmao após rotação
            }

            //se não houver sobrinho à esquerda nem á direita ou se eles forem pretos
            if((irmao->dir == NULL || irmao->dir->cor == 0) && (irmao->esq == NULL || irmao->esq->cor == 0)){
                // mesmos passos do anterior (inverte lado das rotações)
                irmao->cor = 1; 
                removido = paiRemovido;
                paiRemovido = removido->pai;
            }else{
                if(irmao->esq == NULL || irmao->esq->cor == 0){
                    irmao->dir->cor = 0;
                    irmao->cor = 1;
                    rotacaoL(pontArv, irmao);
                    irmao = paiRemovido->esq;
                }

                irmao->cor = paiRemovido->cor;
                paiRemovido->cor = 0;
                irmao->esq->cor = 0;
                rotacaoR(pontArv, paiRemovido);
                removido = Arv->raiz;
            }
        }

    }
    //se o nó removido não for NULL ele fica preto
    if(removido != NULL)
        removido->cor = 0;
    
    // após isso ocorrerá a remoção real do nó
}

void removerAluno(RAIZ* Arv, RAIZ* pontArv, TIPOCHAVE ID) {
    // "atual" ponteiro que percorrerá a árvore
    ALUNO atual = Arv->raiz;

    // caso atual seja o último nó da árvore
    if((atual == pontArv->raiz) && (!atual->dir) && (!atual->esq)){
        free(atual);
        pontArv->raiz = NULL;
        return;
    }

    //Se atual == NULL chegou ao fim da árvore
    while (atual != NULL && atual->id != ID) {
        if(ID < atual->id)
            atual = atual->esq;
        else 
            atual = atual->dir;
    }

    // se atual == NULL a chave não existe na árvore
    if(!atual){
        printf("\nAluno nao encontrado!\n");
        return;
    }

    // se for nó folha
    if((atual->dir == NULL) && (atual->esq == NULL)){
        // se remover uma folha preta precisa corrigir
        if(atual->cor == 0)
            verificaNoPreto(Arv, pontArv, atual, atual->pai);
        
        if(atual->pai->esq == atual) // zera os ponteiros do pai
            atual->pai->esq = NULL;
        else 
            atual->pai->dir = NULL;

        free(atual); // caso seja uma folha 

    // se tiver algum filho
    }else if((atual->dir != NULL) || (atual->esq != NULL)){
        ALUNO aux;

        // aux recebe o filho que existir
        if(atual->dir) 
            aux = atual->dir;
        else 
            aux = atual->esq;

        // procura o menor nó da parte direita
        while(aux->esq != NULL)
            aux = aux->esq; 

        // valor a ser deletado recebe menor valor à direita
        atual->id = aux->id; 
        aux->id = ID; // aux recebe o valor a ser deletado

        RAIZ auxArv;
        // aux recebe o nó que existir
        if(atual->dir) 
            auxArv.raiz = atual->dir;
        else 
            auxArv.raiz = atual->esq;

        removerAluno(&auxArv, pontArv, ID);
    }

}

void inicializarRB(RAIZ* Arv) {
    Arv->raiz = NULL;
}

void destruirAux(ALUNO p) {
    if(p != NULL){
        // percorre nós filhos
        destruirAux(p->esq); 
        destruirAux(p->dir); 

        // libera nó atual
        free(p); 
    }
}

void destruirArvoreRB(RAIZ* Arv) {
    // libera todos os nós da árvore
    destruirAux(Arv->raiz); 

    // define raiz como NULL
    Arv->raiz = NULL; 
}

void calcularAlturaNegraAux(ALUNO p, int altAtual, int *altNegra) {
    if(p == NULL){
        if(*altNegra == -1 || altAtual < *altNegra)
            *altNegra = altAtual;
        
        return;
    }
    
    if(p->cor == 0) // Nó preto
        altAtual++;
    
    calcularAlturaNegraAux(p->esq, altAtual, altNegra);
    calcularAlturaNegraAux(p->dir, altAtual, altNegra);
}

int alturaNegra(RAIZ *Arv) {
    if(Arv == NULL || Arv->raiz == NULL) 
        return -1;
    
    int altNegra = -1;
    calcularAlturaNegraAux(Arv->raiz, 0, &altNegra);

    return altNegra;
}

void contarNos(ALUNO raiz, int *vermelhos, int *pretos) {
    if(raiz == NULL)
        return;

    if(raiz->cor == 1)
        (*vermelhos)++;
    else
        (*pretos)++;

    // busca e conta nas subárvores
    contarNos(raiz->esq, vermelhos, pretos);
    contarNos(raiz->dir, vermelhos, pretos);
}

void calcularPorcentagemNo(ALUNO raiz) {
    // árvore vazia
    if(raiz == NULL)
        return;

    int rubro = 0;
    int negro = 0;

    contarNos(raiz, &rubro, &negro);

    int total = rubro + negro;

    float percentRubro = (float)rubro / total * 100;
    float percentNegro = (float)negro / total * 100;

    printf("\nNos rubros: %.2f%%\nNos negros: %.2f%%\n", percentRubro, percentNegro);
}

void ligarNo(RAIZ* Cabeca, char* nome, FILE* fdot) {
    if(Cabeca->raiz == NULL) 
        return;
    ALUNO Arv = Cabeca->raiz;

    fdot = fopen(nome, "a+");
    fprintf(fdot, "\n\t\t\"%d\\ncor:%d\"", Arv->id, Arv->cor);

    if(Arv->esq != NULL){
        fprintf(fdot, "\n\t\t\"%d\\ncor:%d\" -> \"%d\\ncor:%d\"\n", Arv->id, Arv->cor, Arv->esq->id, Arv->esq->cor);
        if (Arv->esq->cor == 0) // cor = 0 é preto
            fprintf(fdot, "\t\t\"%d\\ncor:%d\"  [style=filled, fillcolor=black, fontcolor=white];\n", Arv->esq->id, Arv->esq->cor);
        else // cor = 1 é vermelho
            fprintf(fdot, "\t\t\"%d\\ncor:%d\"  [style=filled, fillcolor=red, fontcolor=black];\n", Arv->esq->id, Arv->esq->cor);

        RAIZ aux;
        aux.raiz = Arv->esq;

        ligarNo(&aux, nome, fdot);
    }

    if(Arv->dir != NULL){
        fprintf(fdot, "\n\t\t\"%d\\ncor:%d\" -> \"%d\\ncor:%d\"\n", Arv->id, Arv->cor, Arv->dir->id, Arv->dir->cor);
        if (Arv->dir->cor == 0) // cor = 0 é preto
            fprintf(fdot, "\t\t\"%d\\ncor:%d\"  [style=filled, fillcolor=black, fontcolor=white];\n", Arv->dir->id, Arv->dir->cor);
        else // cor = 1 é vermelho
            fprintf(fdot, "\t\t\"%d\\ncor:%d\"  [style=filled, fillcolor=red, fontcolor=black];\n", Arv->dir->id, Arv->dir->cor);

        RAIZ aux;
        aux.raiz = Arv->dir;

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

int main(){

    RAIZ Arv;
    inicializarRB(&Arv);

    inserirAluno(&Arv, 10);
    inserirAluno(&Arv, 20);
    inserirAluno(&Arv, 30);
    inserirAluno(&Arv, 40);
    inserirAluno(&Arv, 50);
    //inserirAluno(&Arv, 20); // nó já inserido
    inserirAluno(&Arv, 9);
    inserirAluno(&Arv, 8);
    inserirAluno(&Arv, 6);
    inserirAluno(&Arv, 7);
    inserirAluno(&Arv, 5);
    inserirAluno(&Arv, 60);

    removerAluno(&Arv, &Arv, 20);
    removerAluno(&Arv, &Arv, 9);
    removerAluno(&Arv, &Arv, 50);
    //removerAluno(&Arv, &Arv, 40);

    // impressão ARB em ordem
    printf("\nArvore Rubro-Negra em ordem:\n");
    exibirArvoreEmOrdem(Arv.raiz);
    printf("\n");
    
    printf("\nAltura total da arvore: %d\n", alturaTotal(Arv.raiz));
    printf("Altura Negra da arvore: %d\n", alturaNegra(&Arv));

    calcularPorcentagemNo(Arv.raiz);

    FILE* ArqDot = inicializarDot("arvore.dot");
    ligarNo(&Arv, "arvore.dot", ArqDot);
    terminarDot("arvore.dot");

    // libera memória alocada para a árvore (remove todos os nós)
    destruirArvoreRB(&Arv);

    return 0;
}
