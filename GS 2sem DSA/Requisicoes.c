#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>

// Tamanhos máximos das strings
#define TAMANHO_NOME 50
#define TAMANHO_TEMA 80

// Estrutura do Nó
typedef struct NoRequisicao {
    char nome[TAMANHO_NOME];   // Nome do aluno
    char tema[TAMANHO_TEMA];   // Tema da mentoria
    struct NoRequisicao *proximo; // Ponteiro para o próximo nó (o que vem depois)
} NoRequisicao;

// Estrutura da Fila
typedef struct FilaMentoria {
    NoRequisicao *frente; // Ponteiro para o primeiro nó da fila (o mais antigo, a ser atendido)
    NoRequisicao *fundo;  // Ponteiro para o último nó da fila (o mais recente, onde se insere)
} FilaMentoria;

// Função para criar um novo nó de requisição
NoRequisicao* criarNo(const char* nomeAluno, const char* temaMentoria) {
    NoRequisicao* novoNo = (NoRequisicao*)malloc(sizeof(NoRequisicao));
    if (novoNo == NULL) {
        printf("Erro de alocação de memória.\n");
        exit(1);
    }
    // Copia os dados
    strncpy(novoNo->nome, nomeAluno, TAMANHO_NOME - 1);
    novoNo->nome[TAMANHO_NOME - 1] = '\0';
    
    strncpy(novoNo->tema, temaMentoria, TAMANHO_TEMA - 1);
    novoNo->tema[TAMANHO_TEMA - 1] = '\0';
    
    novoNo->proximo = NULL; 
    return novoNo;
}

// Função para inicializar a fila
void inicializarFila(FilaMentoria* fila) {
    fila->frente = NULL;
    fila->fundo = NULL;
}

// Função para verificar se a fila está vazia
int estaVazia(FilaMentoria* fila) {
    return (fila->frente == NULL);
}

// Função para limpar o buffer de entrada
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// 1. Enfileirar novos pedidos - Insere no Fundo
void enfileirar(FilaMentoria* fila, const char* nome, const char* tema) {
    NoRequisicao* novoNo = criarNo(nome, tema);
    // Caso 1: Fila Vazia
    if (estaVazia(fila)) {
        fila->frente = novoNo;
        fila->fundo = novoNo;
    } 
    // Caso 2: Fila Não Vazia
    else {
        // O 'proximo' do nó que era o fundo aponta para o novo nó
        fila->fundo->proximo = novoNo;
        // O novo nó se torna o fundo da fila
        fila->fundo = novoNo;
    }
    printf("\nPedido enfileirado: %s (Tema: %s).\n", nome, tema);
}

// 2. Atender o próximo da fila - Remove da Frente
void desenfileirar(FilaMentoria* fila) {
    // Verifica se a fila está vazia
    if (estaVazia(fila)) {
        printf("\nErro: A fila de requisições está vazia. Não há pedidos para atender.\n");
        return;
    }
    // O nó a ser removido é o da frente
    NoRequisicao* noAtendido = fila->frente;
    
    printf("\nRequisição atendida! (Saindo da Frente)\n");
    printf("  Nome: %s\n", noAtendido->nome);
    printf("  Tema: %s\n", noAtendido->tema);

    // O novo nó da frente passa a ser o próximo do nó atendido
    fila->frente = fila->frente->proximo;
    // Se a fila ficou vazia após a remoção, o fundo também deve ser NULL
    if (fila->frente == NULL) {
        fila->fundo = NULL;
    }
    // Libera a memória do nó atendido
    free(noAtendido);
}

// 3. Exibir os que ainda aguardam atendimento
void exibirFila(FilaMentoria* fila) {
    if (estaVazia(fila)) {
        printf("\nRequisições pendentes:\n  (A fila está vazia. Sem pedidos de mentoria aguardando.)\n");
        return;
    }
    // Ponteiro que percorre a fila, começa na frente
    NoRequisicao* atual = fila->frente;
    int contador = 1;
    printf("\nRequisições pendentes: (Frente para Fundo):\n");
    
    while (atual != NULL) {
        // Marca a frente (o próximo a ser atendido)
        if (atual == fila->frente) {
            printf("  %d. Frente/próximo: %s (Tema: %s)\n", contador, atual->nome, atual->tema);
        } else {
            printf("  %d. %s (Tema: %s)\n", contador, atual->nome, atual->tema);
        }
        atual = atual->proximo; // Move para o próximo nó
        contador++;
    }
}

// Função para liberar a memória de toda a fila (IMPORTANTE!)
void liberarFila(FilaMentoria* fila) {
    NoRequisicao* atual = fila->frente;
    NoRequisicao* proximo;

    while (atual != NULL) {
        proximo = atual->proximo; // Guarda o ponteiro do próximo
        free(atual);              // Libera a memória do nó atual
        atual = proximo;          // Move para o próximo nó
    }
    fila->frente = NULL;
    fila->fundo = NULL;
}

// --- Função Principal e Menu ---

int main() {
    // Configura o console para usar UTF-8
    SetConsoleOutputCP(CP_UTF8); // Define o console para usar UTF-8
    SetConsoleCP(CP_UTF8); // Define a entrada do console para UTF-8

    // Configura a localidade para UTF-8 no sistema
    setlocale(LC_ALL, "pt_BR.UTF-8");

    // Inicializa a fila com frente e fundo apontando para NULL (fila vazia)
    FilaMentoria fila;
    inicializarFila(&fila); 

    int escolha;
    char nomeBuffer[TAMANHO_NOME];
    char temaBuffer[TAMANHO_TEMA];

    do {
        // 4. Oferecer um menu ao usuário
        printf("\n==============================================\n");
        printf("     Fila: Processamento de Mentorias\n");
        printf("==============================================\n");
        printf("1. Enfileirar novo pedido (Nova Requisição)\n");
        printf("2. Atender o próximo da fila (Desenfileirar)\n");
        printf("3. Exibir requisições pendentes\n");
        printf("0. Sair e encerrar o programa\n");
        printf("----------------------------------------------\n");
        printf("Escolha uma opção: ");
        // Lê a escolha do usuário
        if (scanf("%d", &escolha) != 1) {
            printf("\nOpção inválida. Por favor, digite um número.\n");
            limparBuffer();
            escolha = -1; 
            continue;
        }
        limparBuffer(); // Limpa o buffer após o scanf

        switch (escolha) {
            case 1:
                printf("\nNome do aluno: ");
                if (fgets(nomeBuffer, TAMANHO_NOME, stdin) != NULL) {
                    nomeBuffer[strcspn(nomeBuffer, "\n")] = 0; 
                } else break;
                printf("Tema da mentoria: ");

                if (fgets(temaBuffer, TAMANHO_TEMA, stdin) != NULL) {
                    temaBuffer[strcspn(temaBuffer, "\n")] = 0; 
                } else break;

                if (strlen(nomeBuffer) > 0 && strlen(temaBuffer) > 0) {
                    enfileirar(&fila, nomeBuffer, temaBuffer);
                } else {
                    printf("Nome e tema não podem ser vazios.\n");
                }
                break;
            case 2:
                desenfileirar(&fila);
                break;
            case 3:
                exibirFila(&fila);
                break;
            case 0:
                printf("\nEncerrando o programa. Memória da fila liberada.\n");
                liberarFila(&fila); // Libera toda a memória antes de sair
                break;
            default:
                printf("\nOpção inválida. Por favor, escolha uma opção entre 0 e 3.\n");
                break;
        }
    } while (escolha != 0);

    return 0;
}