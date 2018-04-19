#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Define a estrutura do no
typedef struct no {
  int valor;
  int balanceamento;   /* hDireita - hEsquerda */
  struct no *esquerda, *direita;
} No;

/*
 * Calcula a altura da arvore ou de uma subarvore
 * A funcao eh chamada recursivamente aa esquerda e aa direita (para identificar de qual lado a altura eh maior)
 */
int altura(No* t) {
    if (t == NULL) {
        return 0;
    }
    int hEsquerda = altura(t->esquerda);
    int hDireita = altura(t->direita);
    return hEsquerda > hDireita ? hEsquerda + 1 : hDireita + 1; // Retorna a maior altura calculada mais o pr�prio n�
}

/*
 * Cria um 'no' na arvore
 * o primeiro parametro eh o valor do 'no' que sera criado
 * o segundo eh o numero da linha no arquivo
 * os demais sao os 'nos' da esquerda e da direita
*/
No* cria(int valor, No* esquerda, No* direita) {
    No* n = (No*) malloc (sizeof(No));
    n->valor = valor;
    n->balanceamento = altura(direita) - altura(esquerda);
    n->esquerda = esquerda;
    n->direita = direita;
    return n;
}

/*
 * Bonus - verifica se a arvore (ou 'no') esta balanceada(o)
 */
int verifica_AVL(No* t) {
    if (t == NULL)
        return 1;
    return abs(altura(t->direita) - altura(t->esquerda)) <= 1;
}

void listagem(No *t) {

  // Verifica se a arvore esta vazia
   if(t != NULL){
     listagem(t->esquerda);
     printf("\n%d", t->valor);
     listagem(t->direita);
   }
}


/*
 * verifica_escreve primeiro verifica se a arvore esta balanceada (eh AVL)
 * e depois chama a funcao escreve que imprime os elementos de forma ordenada
 */
void verifica_escreve(No* t) {
    if (t == NULL) {
      printf("\nArvore vazia!\n");
    }else if (!verifica_AVL(t)) {
      printf("\nArvore nao esta balanceada(AVL)!\n");
    }else{
      listagem(t);
    }
    printf("\n\nPressione a tecla Enter para retornar.\n");
}

/*
 * rotacao do 'no' a esquerda apos uma inclusao a esquerda
 *
 */
void esquerdaEsquerda(No** r) {
  No* b = *r;
  No* a = b->esquerda;
  b->esquerda = a->direita;
  a->direita = b;
  a->balanceamento = 0;
  b->balanceamento = 0;
  *r = a;
}

/*
 *
 *
 */
void direitaDireita(No** r) {
  No* a = *r;
  No* b = a->direita;
  a->direita = b->esquerda;
  b->esquerda = a;
  a->balanceamento = 0;
  b->balanceamento = 0;
  *r = b;
}

/*
 * rotacao do 'no' a direita apos uma inclusao a esquerda
 *
 */
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

/*
 * rotacao do 'no' a direita apos uma inclusao a esquerda
 *
 */
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


/*
 * aux_insere ira incluir valor no 'no' "t", quando este for nulo
 * enquanto o 'no' n�o for nulo, a funcao ira comparar o valor de "t" com o valor a ser inserido
 * e chamar a si propria recursivamente ate encontrar o ponto da arvore onde "valor" deve ser incluido
 *
 *   *cresceu indica se a insercao teve sucesso
 */
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

    // Se a inclusao aa direita tiver sucesso, faz o balanceamento
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

/*
 * insere um 'no' na arvore
 * chama aux_insere que eh a funcao que identificara o 'no' da arvore onde o valor deve ser incluido
 * se o valor ja existir, nao sera incluido novamente e o retorno sera 0. Se for incluido, o retorno sera 1
 */
int insere(No **t) {
  int cresceu, valor;
  printf("Digite o valor que deseja inserir: " );
  scanf("%d", &valor );
  return aux_insere(t, valor, &cresceu);
}


/*
 *  localizaValor eh um metodo recursivo que percorre a arvore cujo 'no' raiz foi recebido no primeiro parametro
 *  em busca do valor informado no segundo parametro
 */
// No* localizaValor(No *t, int chavePesquisa) {
//     // Se nao encontrou, retorna NULL
//     if (t == NULL) {
//         return t;
//     }
//     // Se encontrou retorna o 'no' que contem o valor
//     if (valor == (*t)->valor) {
//         return t;
//     }
//
//     if (valor < (*t)->valor) {
//         // Se o valor eh MENOR que o valor do 'no' atual, a chamada recursiva ocorrera a ESQUERDA
//         return localizaValor(t->esquerda, chavePesquisa);
//     } else {
//         // Se o valor eh MAIOR que o valor do 'no' atual, a chamada recursiva ocorrera a DIREITA
//         return localizaValor(t->direita, chavePesquisa);
//     }
//
// }


/*
*  Exibe as opcoes ao usuario
*/
char menuPrincipal() {
    int opcao;
    printf("|--------------------------|\n");
    printf("|        Arvore AVL        |\n");
    printf("|--------------------------|\n");
    printf("\nEscolha sua opcao\n");
    printf("1 - Inserir valor na arvore\n");
    printf("2 - Listar arvore\n");
    printf("0 - Sair\n");
    printf("\nInforme o numero da operacao desejada: ");
    scanf("%d", &opcao);
    return opcao;
}

/*
 * main controla a execucao do programa
 *
 */
int main() {
    int opcao;
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
            case 0:
                exit(0);
            default:
                printf("\n\nOpcao inexistente!\n");
        }
    } while (1);
    return 0;
}
