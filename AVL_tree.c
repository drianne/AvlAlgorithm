#include <stdio.h>
#include <stdlib.h>

// Define a estrutura do no
struct arvore{
    int valor, fb;  /* fb =  hDireita - hEsquerda */

    struct arvore *esquerda;
    struct arvore *direita;
    struct arvore *pai;
};

struct arvore *raiz = NULL; //Setando raiz da árvore como nulo

//Declaração de funções

void menu();
void insere();
void exibir_pos(struct arvore*);
void remove_p();
void remover(struct arvore*);
struct arvore* menor_direita(struct arvore*);
struct arvore* buscar(int);
int altura(struct arvore*);
struct arvore* balanceia(struct arvore*);
struct arvore* rot_direita(struct arvore*);
struct arvore* rot_esquerda(struct arvore*);

void menu(){

    printf("|--------------------------|\n");
    printf("|        Arvore AVL        |\n");
    printf("|--------------------------|\n");
    printf("\nEscolha sua opcao\n");
    printf("1 - Inserir valor na arvore\n");
    printf("2 - Deletar Valor\n");
    printf("3 - Buscar\n");
    printf("4 - Exibir - Pos Ordem\n");
    printf("0 - Sair\n\n");

    printf("\nInforme o numero da operacao desejada: ");

}

void insere(){
    struct arvore *aux1 = NULL, *aux2 = NULL;
    int valor;

    printf("Digite o numero a inserir: ");
    scanf("%d", &valor);

    // Se raiz estiver nula ( árvore vazia )
    if(!raiz){
        raiz = (struct arvore*) malloc(sizeof(struct arvore)); // Aloca um nó em memória e coloca ponteiro da raiz para apontar para ele.

        //Checa se foi possível alocar
        if(!raiz){
            printf("Erro ao alocar =(\n");
            exit(1);
        }

        //preenche o nó com os dados iniciais
        raiz->valor = valor;
        raiz->esquerda = NULL;
        raiz->direita = NULL;
        raiz->pai = NULL;

        raiz->fb = 0;
    }
    else{
        aux1 = raiz;
        aux2 = aux1;

        while(aux2){
          //Checa se é pra enviar pra direita ou esquerda
            if(valor < aux2->valor){
                aux2 = aux2->esquerda;
                //Testa se local está vazio
                if(!aux2){
                    aux1->esquerda = (struct arvore*) malloc(sizeof(struct arvore));

                    if(!aux1->esquerda){
                        printf("Erro ao alocar =(\n");
                        exit(1);
                    }

                    aux2 = aux1->esquerda;

                    aux2->esquerda = NULL;
                    aux2->direita = NULL;
                    aux2->pai = aux1;
                    aux2->valor = valor;
                    break;
                }//Se não tiver vazio, ando
                else{
                    aux1 = aux2;
                }
            }

            else{ //Para outro lado
                aux2 = aux2->direita;

                if(!aux2){
                    aux1->direita = (struct arvore*) malloc(sizeof(struct arvore));

                    if(!aux1->direita){
                        printf("Erro ao alocar =(\n");
                        exit(1);
                    }

                    aux2 = aux1->direita;

                    aux2->esquerda = NULL;
                    aux2->direita = NULL;
                    aux2->pai = aux1;
                    aux2->valor = valor;
                    break;
                }

                else{
                    aux1 = aux2;
                }
            }
        }
    }

    printf("Elemento Inserido!\n");

    //Calculando e balanceando após inserção
    if(aux2){
        while(aux2){
            aux2->fb = altura(aux2->direita) - altura(aux2->esquerda);

            if(aux2->fb > 1 || aux2->fb < -1){
                aux2 = balanceia(aux2);
            }

            aux2 = aux2->pai;
        }
    }
}

void exibir_pos(struct arvore *aux){
    if(!raiz){
        printf("Arvore vazia!");
        return;
    }

    if(aux){
        exibir_pos(aux->esquerda);
        exibir_pos(aux->direita);
        printf("%d(%d)\n\n", aux->valor, aux->fb);
    }
}

//Lê e busca número a ser removido
void remove_chamada(){
    struct arvore *aux;
    int n;

    printf("Digite o numero que deseja remover: ");
    scanf("%d", &n);

    aux = buscar(n);

    if(aux){
        remover(aux);
    }

    else{
        printf("O numero nao existe na arvore!\n");
    }
}

void remover(struct arvore *aux){
    struct arvore *aux2, *salva = aux->pai;
    //Se não for raiz
    if(aux != raiz){
        // Tem 2 filhos
        if(!aux->esquerda && !aux->direita){
            if(aux->valor < aux->pai->valor){
                aux->pai->esquerda = NULL;
            }
            else{
                aux->pai->direita = NULL;
            }
            free(aux);
        }
        //Se só tem filho à direita
        else if(!aux->direita){
            if(aux->valor < aux->pai->valor){
                aux->pai->esquerda = aux->esquerda;
            }
            else{
                aux->pai->direita = aux->esquerda;
            }
            aux->esquerda->pai = aux->pai;
            free(aux);
        }
        //Só tem filhos à esquerda
        else if(!aux->esquerda){
            if(aux->valor < aux->pai->valor){
                aux->pai->esquerda = aux->direita;
            }
            else{
                aux->pai->direita = aux->direita;
            }
            aux->direita->pai = aux->pai;
            free(aux);
        }
        //Mais à esquerda possível da direita
        else{
            aux2 = menor_direita(aux->direita);
            aux->valor = aux2->valor;

            remover(aux2);
        }
    }
    //Se for raiz
    else{
        //Com 2 filhos
        if(!aux->esquerda && !aux->direita){
            free(aux);
            raiz = NULL;
        }
        //Com filho à direita
        else if(!aux->direita){
            aux->esquerda->pai = NULL;
            raiz = aux->esquerda;
            free(aux);
        }
        //Com filho à esquerda
        else if(!aux->esquerda){
            aux->direita->pai = NULL;
            raiz = aux->direita;
            free(aux);
        }
        else{
            aux2 = menor_direita(aux->direita);
            aux->valor = aux2->valor;

            remover(aux2);
        }
    }
    //Recalcula valanceamento, balenceia a árvore
    if(salva){
        while(salva){
            salva->fb = altura(salva->direita) - altura(salva->esquerda);

            if(salva->fb > 1 || salva->fb < -1){
                salva = balanceia(salva);
            }

            salva = salva->pai;
        }
    }
}

//Pega menor elemento à direita de um nó
struct arvore* menor_direita(struct arvore *aux){
    while(aux->esquerda){
        aux = aux->esquerda;
    }
    return aux;
}

//Busca um elemento
struct arvore* buscar(int valor){
    struct arvore *aux;

    aux = raiz;

    while(aux && aux->valor != valor){
        if(valor < aux->valor){
            aux = aux->esquerda;
        }

        else{
            aux = aux->direita;
        }
    }

    return aux;
}

//Calcula altura da árvore
int altura(struct arvore *aux){
    if(!aux){
        return 0;
    }

    int alturaEsquerda = altura(aux->esquerda);
    int alturaDireita = altura(aux->direita);

    return (alturaEsquerda > alturaDireita) ? alturaEsquerda+ 1: alturaDireita + 1;
}

struct arvore* rot_direita(struct arvore *aux){
    struct arvore *esq = aux->esquerda;

    aux->esquerda = esq->direita;
    esq->direita = aux;

    if(aux->esquerda){
        aux->esquerda->pai = aux;
    }

    esq->pai = aux->pai;
    aux->pai = esq;

    if(aux == raiz){
        raiz = esq;
    }

    if(esq->pai){
        if(esq->valor < esq->pai->valor){
            esq->pai->esquerda = esq;
        }
        else{
            esq->pai->direita = esq;
        }
    }

    esq->fb = altura(esq->direita) - altura(esq->esquerda);
    aux->fb = altura(aux->direita) - altura(aux->esquerda);

    return esq;
}

struct arvore* rot_esquerda(struct arvore *aux){
    struct arvore *dir = aux->direita;

    aux->direita = dir->esquerda;
    dir->esquerda = aux;

    if(aux->direita){
        aux->direita->pai = aux;
    }

    dir->pai = aux->pai;
    aux->pai = dir;

    if(aux == raiz){
        raiz = dir;
    }

    if(dir->pai){
        if(dir->valor < dir->pai->valor){
            dir->pai->esquerda = dir;
        }
        else{
            dir->pai->direita = dir;
        }
    }

    dir->fb = altura(dir->direita) - altura(dir->esquerda);
    aux->fb = altura(aux->direita) - altura(aux->esquerda);
    return dir;
}

//Balanceando a árvore
struct arvore* balanceia(struct arvore *aux){
    if(aux->fb < -1){
        if(aux->esquerda->fb > 0){
            aux->esquerda = rot_esquerda(aux->esquerda);
        }

        aux = rot_direita(aux);
    }

    else if(aux->fb > 1){
        if(aux->direita->fb < 0){
            aux->direita = rot_direita(aux->direita);
        }

        aux = rot_esquerda(aux);
    }

    return aux;
}

int main(){
    struct arvore *busca;
    int op, n;

    while(1){
        menu();

        scanf("%d", &op);

        switch(op){
            case 1: insere();
                    break;

            case 2: remove_chamada();
                    break;

            case 3: printf("Digite o numero a procurar: ");
                    scanf("%d", &n);

                    busca = buscar(n);

                    if(busca){
                        printf("Elemento encontrado: %d\n", busca->valor);
                    }
                    else{
                        printf("Elemento nao encontrado!\n");
                    }

                    busca = NULL;
                    break;

            case 4: exibir_pos(raiz);
                    break;
            case 0:
                exit(0);
                break;

            default: printf("Opcao incorreta!\n");
                     break;
        }
    }
}
