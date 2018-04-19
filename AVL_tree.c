/*
 *  Arvore de busca balanceamentoanceada AVL (Adelson-Velskii e Landis)
 *
 *  Autor: Lucas Arthur Lermen
 *  Matrícula: 16/0012961
 *  Disciplina: EDA (Estrutura de Dados e Algoritmos)
 *  UnB / FGA
 *
 *
 */

#include <stdlib.h>
#include <string.h>

//Se o sistema for Windows adiciona determinada biblioteca, e definindo comandos de limpar e esperar
#ifdef WIN32
    #include <stdio.h>
    #include <windows.h>
    #define LIMPA_TELA system("cls")
    #define LIMPA_BUFFER fflush(stdin)
//Sen�o for Windows (ex.: Linux)
#else
    #include <stdio_ext.h>
    #include <unistd.h>
    #define LIMPA_TELA system("clear")
    #define LIMPA_BUFFER __fpurge(stdin)
#endif

FILE *arquivoVideo;
char nomeArquivoVideo[80];


/*
*  Abre o arquivo para leitura e gravacao (r+)
*  Em modo binario (b)
*  Se o arquivo nao existir, sera criado
*/
FILE * abrirLeituraGravacao(char nomeArquivo[80]) {
    FILE * arq = NULL;
    if((arq = fopen(nomeArquivo, "rb+")) == NULL) {
        printf("\n O arquivo '%s' nao pode ser aberto!!\n", nomeArquivo);
        getchar();
        exit(1);
    }
    return arq;
}

/*
* Fecha o arquivo que estiver aberto
* Esta funcao deve ser chamada para evitar que fique aberta a conexao entre o programa e o arquivo em disco
*/
void fecharArquivo(FILE * arq) {
    fclose(arq);
}

// Define a estrutura do no
typedef struct no {
  char chaveValor[7];
  int linhaArquivo;
  int balanceamento;   /* hDireita - hEsquerda */
  struct no *esquerda, *direita;
} No;

// Define a estrutura do video
typedef struct video {
    char chaveValor[7];
    char titulo[255];
    char midia[50];
    float preco;
    char genero[255];
} Video;

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
No* cria(char* chaveValor, int linhaArquivo, No* esquerda, No* direita) {
    No* n = (No*) malloc (sizeof(No));
    strcpy(n->chaveValor, chaveValor);
    n->linhaArquivo = linhaArquivo;
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

/*
 * escreve os valores da arvore em ordem
 * funcao recursiva que escreve primeiro os valores aa esquerda, depois o proprio valor e por ultimo os valores aa direita
 */
void escreve(No* r) {
    if (r != NULL) {
        escreve(r->esquerda);
        printf("%s(%d)(%d) \n", r->chaveValor, r->balanceamento, r->linhaArquivo);
        escreve(r->direita);
    }
}

/*
 * verifica_escreve primeiro verifica se a arvore esta balanceada (eh AVL)
 * e depois chama a funcao escreve que imprime os elementos de forma ordenada
 */
void verifica_escreve(No* t) {
    if (t == NULL) {
        printf("\nArvore vazia!\n");
    }
    if (!verifica_AVL(t)) {
        printf("\nArvore nao esta balanceada(AVL)!\n");
    }
    escreve(t);
    printf("\n");
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
 * aux_insere ira incluir chaveValor e linhaArquivo no 'no' "t", quando este for nulo
 * enquanto o 'no' n�o for nulo, a funcao ira comparar o valor de "t" com o valor a ser inserido
 * e chamar a si propria recursivamente ate encontrar o ponto da arvore onde "chaveValor" deve ser incluido
 *
 *   *cresceu indica se a insercao teve sucesso
 */
int aux_insere(No** t, char* chaveValor, int linhaArquivo, int *cresceu) {
    // Se o 'no' atual for nulo, inclui os valores neste 'no'
    if (*t == NULL) {
        *t = cria(chaveValor, linhaArquivo, NULL, NULL);
        *cresceu = 1;
        return 1;
    }

    // Se o valor a ser inserido ja existir na arvore, nao sera incluido
    if (strcasecmp(chaveValor, (*t)->chaveValor) == 0) {
        return 0;
    }

    // Se o valor a ser inserido eh MENOR que o valor do 'no' atual, a chamada recursiva ocorrera a ESQUERDA
    if (strcasecmp(chaveValor, (*t)->chaveValor) < 0) {
        // Se a inclusao aa esquerda tiver sucesso, faz o balanceamento
        if (aux_insere(&(*t)->esquerda, chaveValor, linhaArquivo, cresceu)) {
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
    if (aux_insere(&(*t)->direita, chaveValor, linhaArquivo, cresceu)) {
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
int insere(No **t, char *chaveValor, int linhaArquivo) {
  int cresceu;
  return aux_insere(t, chaveValor, linhaArquivo, &cresceu);
}

/*
 *  carrega o arquivo informado na memoria
 *  o arquivo eh carregado na arvore cujo 'no' raiz eh o retorno deste metodo
 */
No* carrega() {
    char arquivoDigitado;
    printf("\nDigite o nome do arquivo:\n");
    scanf("%s", &arquivoDigitado );
    // Validar o conteudo digitado
    strcpy(nomeArquivoVideo, &arquivoDigitado);

    // Verifica se o nome do arquivo foi informado
    if (nomeArquivoVideo == NULL || strcmp(nomeArquivoVideo, "") == 0) {
        printf("\n\nErro: nome do arquivo de video nao informado\n");
        getchar();
        return NULL;
    }
    No *t = NULL;
    char linha[255], chaveValor[7], titulo[255], midia[50], genero[255];
    float preco;

    arquivoVideo = abrirLeituraGravacao(nomeArquivoVideo);
    // Descarta a primeira linha que contem titulos
    int linhaarquivo = 1;
    if (!feof(arquivoVideo)) {
        fgets(linha, sizeof(linha), arquivoVideo);
    }

    // Percorre todo o arquivo
    while(!feof(arquivoVideo)) {
        // Le um registro do arquivo e armazena na arvore
        fscanf(arquivoVideo,"%s %s %s %f %s\n", chaveValor, titulo, midia, &preco, genero);
        linhaarquivo = linhaarquivo + 1;
        insere(&t, chaveValor, linhaarquivo);
    }

    fecharArquivo(arquivoVideo);

    printf("\n\n");
    printf("|-------------------------------------------|\n");
    printf("|  Arquivo carregado e pronto para pesquisa |\n");
    printf("|-------------------------------------------|\n");
    printf("\n\nPressione a tecla Enter para retornar.\n");
    LIMPA_BUFFER;
    getchar();
    return t;
}

/*
 * pesquisaArquivo ira localizar o conteudo existente na linha informada no 'no' recebido como parametro
 * O metodo retorna o video com o conteudo localizado
 */
Video* pesquisaArquivo(No *noValor) {
   if (nomeArquivoVideo == NULL || strcmp(nomeArquivoVideo, "") == 0) {
        printf("\n\nErro: nome do arquivo de video nao informado");
    }
    char linha[255];
    Video *v = (Video*) malloc(sizeof(Video));
    arquivoVideo = abrirLeituraGravacao(nomeArquivoVideo);
    // Descarta as linhas anteriores
    int linhaarquivo = 1;
    while(!feof(arquivoVideo) && (linhaarquivo < (noValor->linhaArquivo))) {
        linhaarquivo = linhaarquivo + 1;
        fgets(linha, sizeof(linha), arquivoVideo);
    }
    if (!feof(arquivoVideo)) {
        // Recupera o conteudo da linha informada
        fscanf(arquivoVideo,"%s %s %s %f %s\n", v->chaveValor, v->titulo, v->midia, &v->preco, v->genero);
    } else {
        // ERRO - NAO DEVE CHEGAR NO FIM
    }
    fecharArquivo(arquivoVideo);
    return v;
}

/*
 *  localizaValor eh um metodo recursivo que percorre a arvore cujo 'no' raiz foi recebido no primeiro parametro
 *  em busca do valor informado no segundo parametro
 */
No* localizaValor(No *t, char* chavePesquisa) {
    // Se nao encontrou, retorna NULL
    if (t == NULL) {
        return t;
    }
    // Se encontrou retorna o 'no' que contem o valor
    if (strcasecmp(chavePesquisa, t->chaveValor) == 0) {
        return t;
    }

    if (strcasecmp(chavePesquisa, t->chaveValor) < 0) {
        // Se o valor eh MENOR que o valor do 'no' atual, a chamada recursiva ocorrera a ESQUERDA
        return localizaValor(t->esquerda, chavePesquisa);
    } else {
        // Se o valor eh MAIOR que o valor do 'no' atual, a chamada recursiva ocorrera a DIREITA
        return localizaValor(t->direita, chavePesquisa);
    }

}

/*
 * IniciaPesquisa eh o metodo que pergunta ao usuario qual valor deseja pesquisar
 */
void iniciaPesquisa(No *t) {

  // Verifica se a arvore esta vazia
  if (t == NULL) {
      printf("\n\nA arvore esta vazia. Use a opcao de carregar primeiro.\n");

  } else {

      // Localizar o valor na arvore
      No *noValor = localizaValor(t, chavePesquisa);

      // Exibe o resultado
      printf("\n");
      printf("|--------------------------|\n");
      printf("|  Resultado               |\n");
      printf("|--------------------------|\n");
      if (noValor == NULL) {
          printf("\n%s nao catalogado!!!\n\n", chavePesquisa);
      } else {
          printf("\n%s localizado na linha %d\n\n", chavePesquisa, noValor->linhaArquivo);

          // Recupera o valor no arquivo
          Video *dadosVideo = pesquisaArquivo(noValor);

          printf("Titulo: %s\n", dadosVideo->titulo);
          printf("Midia: %s\n" , dadosVideo->midia);
          printf("Preco: %.2f\n" , dadosVideo->preco);
          printf("Genero: %s\n", dadosVideo->genero);
      }
  }
  printf("\n\nPressione a tecla Enter para retornar.\n");
  LIMPA_BUFFER;
  getchar();


}
/*
*  Exibe as opcoes ao usuario
*/
char menuPrincipal() {
    char opcao[50];
    LIMPA_TELA;
    printf("|--------------------------|\n");
    printf("|    Catalogo de filmes    |\n");
    printf("|--------------------------|\n");
    printf("\nEscolha sua opcao\n");
    printf("1 - Carregar dados para pesquisa\n");
    printf("2 - Relatorio\n");
    printf("0 - Sair\n");
    printf("\nInforme o numero da operacao desejada: ");
    LIMPA_BUFFER;
    scanf("%s", opcao);
    LIMPA_BUFFER;
    if (strlen(opcao)>1){
      return 'x';
    }
    return opcao[0];
}

/*
 * main controla a execucao do programa
 *
 */
int main() {
    char opcao;
    No *t = NULL;

    do {
        opcao = menuPrincipal();
        switch (opcao){
            case '1':
                t = carrega();
                break;
            case '2':
                iniciaPesquisa(t);
                break;
            case '0':
                exit(0);
            default:
                printf("\n\nOpcao inexistente!\n");
                printf("\nPressione a tecla Enter para tentar novamente.\n");
                LIMPA_BUFFER;
                getchar();
        }
    } while (1);
    return 0;
}
