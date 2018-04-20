#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Define a estrutura do no
typedef struct no {
  int valor;
  int balanceamento;   /* hDireita - hEsquerda */
  struct no *esquerda, *direita;
} No;

 // Calcula a altura da arvore ou de uma subarvore

int altura(No* t) {
    if (t == NULL) {
        return 0;
    }
    int hEsquerda = altura(t->esquerda);
    int hDireita = altura(t->direita);
    return hEsquerda > hDireita ? hEsquerda + 1 : hDireita + 1; // Retorna a maior altura calculada mais o pr�prio n�
}

No *remover_no(No *t);
No *remover_no_arvore(No *t, int valor) ;
No *rotacao_simples_direita(No *t);
No *rotacao_simples_esquerda(No *t);

// Cria um 'no' na arvore

No* cria(int valor, No* esquerda, No* direita) {
    No* n = (No*) malloc (sizeof(No));
    n->valor = valor;
    n->balanceamento = altura(direita) - altura(esquerda);
    n->esquerda = esquerda;
    n->direita = direita;
    return n;
}

 // Verifica se a arvore (ou 'no') esta balanceada(o)

int verifica_AVL(No* t) {
    if (t == NULL)
        return 1;
    return abs(altura(t->direita) - altura(t->esquerda)) <= 1;
}

void listagem(No *t) {
   if(t != NULL){
     listagem(t->esquerda);
     printf("\n%d", t->valor);
     listagem(t->direita);
   }
}

// Verifica se a arvore esta balanceada (eh AVL)
// Chama a funcao escreve para imprimir elementos de forma ordenada

void verifica_escreve(No* temp) {
    if (temp == NULL) {
      printf("\nArvore vazia!\n");
    /*}else if (!verifica_AVL(temp)) {
      printf("\nArvore nao esta balanceada(AVL)!\n");*/
    }else{
      listagem(temp);
      printf("\n");
    }
}


 // rotacao do 'no' a esquerda apos uma inclusao a esquerda

void esquerdaEsquerda(No** r) {
  No* b = *r;
  No* a = b->esquerda;
  b->esquerda = a->direita;
  a->direita = b;
  a->balanceamento = 0;
  b->balanceamento = 0;
  *r = a;
}

void direitaDireita(No** r) {
  No* a = *r;
  No* b = a->direita;
  a->direita = b->esquerda;
  b->esquerda = a;
  a->balanceamento = 0;
  b->balanceamento = 0;
  *r = b;
}

// rotacao do 'no' a direita apos uma inclusao a esquerda

void esquerdaDireita(No** r) {
  No *c = *r;
  No *a = c->esquerda;
  No *b = a->direita;
  c->esquerda = b->direita;
  a->direita = b->esquerda;
  b->esquerda = a;
  b->direita = c;
  switch(b->balanceamento) {
  case -1:
    a->balanceamento = 0;
    c->balanceamento = 1;
    break;
  case 0:
    a->balanceamento = 0;
    c->balanceamento = 0;
    break;
  case +1:
    a->balanceamento = -1;
    c->balanceamento = 0;
    break;
  }
  b->balanceamento = 0;
  *r = b;
}

// rotacao do 'no' a direita apos uma inclusao a esquerda

void direitaEsquerda(No** r) {
  No *a = *r;
  No *c = a->direita;
  No *b = c->esquerda;
  c->esquerda = b->direita;
  a->direita = b->esquerda;
  b->esquerda = a;
  b->direita = c;
  switch(b->balanceamento) {
  case -1:
    a->balanceamento = 0;
    c->balanceamento = 1;
    break;
  case 0:
    a->balanceamento = 0;
    c->balanceamento = 0;
    break;
  case +1:
    a->balanceamento = -1;
    c->balanceamento = 0;
    break;
  }
  b->balanceamento = 0;
  *r = b;
}

No *rotacao_simples_direita(No *t) {
    No *q = t->esquerda;
    t->esquerda = q->direita;
    q->direita = t;
    return q;
}

No *rotacao_simples_esquerda(No *t) {
    No *q = t->direita;
    t->direita = q->esquerda;
    q->esquerda = t;
    return q;
}

// Função que procura local em que novo nó deve ser incluído

int aux_insere(No** t, int valor, int *cresceu) {

    // Se o 'no' atual for nulo, inclui os valores neste 'no'
    if (*t == NULL) {
        *t = cria(valor, NULL, NULL);
        *cresceu = 1;
        return 1;
    }

    // Se o valor a ser inserido ja existir na arvore, nao sera incluido
    if (valor == (*t)->valor) {
        return 0;
    }

    // Se o valor a ser inserido eh MENOR que o valor do 'no' atual, a chamada recursiva ocorrera a ESQUERDA
    if (valor < (*t)->valor) {
        // Se a inclusao aa esquerda tiver sucesso, faz o balanceamento
        if (aux_insere(&(*t)->esquerda, valor, cresceu)) {
            if (*cresceu) {
                switch ((*t)->balanceamento) {
                case -1:
                    if ((*t)->esquerda->balanceamento == -1) {
                        esquerdaEsquerda(t);
                    } else {
                        esquerdaDireita(t);
                    }
                    *cresceu = 0;
                    break;
                case 0:
                    (*t)->balanceamento = -1;
                    *cresceu = 1;
                    break;
                case +1:
                    (*t)->balanceamento = 0;
                    *cresceu = 0;
                    break;
                }
            }
            return 1;
        } else {
            return 0;
        }
    }

    // Se chegou aqui eh porque o valor a ser inserido eh MAIOR que o valor do 'no' atual,
    // logo a chamada recursiva ocorrera a DIREITA

    // Se a inclusao a direita tiver sucesso, faz o balanceamento
    if (aux_insere(&(*t)->direita, valor, cresceu)) {
        if (*cresceu) {
            switch ((*t)->balanceamento) {
            case -1:
                (*t)->balanceamento = 0;
                *cresceu = 0;
                break;
            case 0:
                (*t)->balanceamento = +1;
                *cresceu = 1;
                break;
            case +1:
                if ((*t)->direita->balanceamento == +1) {
                    direitaDireita(t);
                } else {
                    direitaEsquerda(t);
                }
                *cresceu = 0;
                break;
            }
        }
        return 1;
    } else {
        return 0;
    }

}

// insere um 'no' na arvore
// Procura local onde nó será incluído

int insere(No **t) {
  int cresceu, valor;
  printf("Digite o valor que deseja inserir: " );
  scanf("%d", &valor );
  return aux_insere(t, valor, &cresceu);
}

//Funçães que retorna o endereço do menor nó da árvore.
No *menor_no_arvore(No *t) {
    No *aux = t;
    while (aux->esquerda != NULL) {  //Procurar o nó mais a esquerda (menor nó da árvore).
        aux = aux->esquerda;
    }
    return aux;                 //Retornar o endereço do menor nó da árvore.
}

//Função que verifica se um nó está balanceado e faz as rotações necessárias.
No *verifica_balanceamento(No *t) {
    int fb;
    fb = verifica_AVL(t);    //Após inserir o nó para esq ou dir, verificar Fator de bal.
    if (fb < -1) {
        if (verifica_AVL(t->esquerda) > 0) {      //Rotação dupla a direita.
            t->esquerda = rotacao_simples_esquerda(t->esquerda);
        }
        t = rotacao_simples_direita(t);             //Rotação simples a esquerda.
    }
    else if (fb > 1) {
        if (verifica_AVL(t->direita) < 0) {      //Rotação dupla a esquerda.
            t->direita = rotacao_simples_direita(t->direita);
        }
        t = rotacao_simples_esquerda(t);            //Rotação simples a direita.
    }
    return t;
}

//Função que remove um nó que é folha (não possui filhos).
No *remover_folha(No *t) {
    printf("\nRegistro %d removido com sucesso!\n", t->valor);
    free(t);
    return NULL;
}

No *remover_1filho_esquerda(No *t) {
    No *aux = t->esquerda;
    printf("\nRegistro %d removido com sucesso!\n", t->valor);
    free(t);
    return aux;
}

No *remover_1filho_direita(No *t) {
    No *aux = t->direita;
    printf("\nRegistro %d removido com sucesso!\n", t->valor);
    free(t);
    return aux;
}

No *remover_2filhos(No *t) {
    No *aux;
    int auxx;
    aux = menor_no_arvore(t->direita);  //auxiliar recebe o endereço do menor nó da árvore.
    auxx = aux->valor;                     //guardar o valor do auxiliar em uma variável x.
    t = remover_no_arvore(t->esquerda,auxx);     //remover da árvore o nó que substituirá o nó com 2 filhos.

  t->valor = auxx;                       //substituir o valor do nó com 2 filhos.
  return t;


}

//Função para verificar o tipo de nó que será removido.
No *remover_no(No *t) {
    //Verificar se o nó é uma folha (Não possui filhos).
    if (t->direita == NULL && t->esquerda == NULL) {
        t = remover_folha(t);
    }
    else {
        if (t->direita == NULL) {
            //printf("O no %d possui 1 filho para esquerda.\n", r->n);
            t = remover_1filho_esquerda(t);
        }
        else {
            if (t->esquerda == NULL) {
                //printf("O no %d possui 1 filho para direita.\n", r->n);
                t = remover_1filho_direita(t);
            }
            else {
                t = remover_2filhos(t);
            }
        }
    }
    return t;
}

No *remover_no_arvore(No *t, int valor) {
    //Verificar se a árvore não está vazia.
    if (t != NULL) {
        //Se a árvore não está vazia, buscar o elemento x.
        if (t->valor == valor) {    //1 - Verificar se o elemento x está na raiz.
            t = remover_no(t);  //Chamar a função para remover o nó.
            t = NULL;
        }
        else {
            //Se x não está na raiz, verificar para qual lado x está.
            if (valor < t->valor) {
                //Fazer chamada recursiva para a esquerda.
                t->esquerda = remover_no_arvore(t->esquerda, valor);
            }
            else {
                //Fazer chamada recursiva para a direita.
                t->direita = remover_no_arvore(t->direita, valor);
            }
            t = verifica_balanceamento(t);
            //Verificar balanceamento da árvore.
        }
    }
    else {
        printf("Elemento nao encontrado: %d\n", valor);
    }
    return t;
}

char menuPrincipal() {
    int opcao;
    printf("|--------------------------|\n");
    printf("|        Arvore AVL        |\n");
    printf("|--------------------------|\n");
    printf("\nEscolha sua opcao\n");
    printf("1 - Inserir valor na arvore\n");
    printf("2 - Listar arvore\n");
    printf("3 - Deletar Valor\n");
    printf("0 - Sair\n");
    printf("\nInforme o numero da operacao desejada: ");
    scanf("%d", &opcao);
    return opcao;
}

int main() {
    int opcao, remove;
    No *t = NULL;

    do {
        opcao = menuPrincipal();
        switch (opcao){
            case 1:
                insere(&t);
                break;
            case 2:
                verifica_escreve(t);
                break;
            case 3:
                printf("\nDigite o valor que deseja remover da arvore: \n");
                scanf("%d", &remove);
                remover_no_arvore(t, remove);
                break;
            case 0:
                exit(0);
            default:
                printf("\n\nOpcao inexistente!\n");
        }
    } while (1);
    return 0;
}
