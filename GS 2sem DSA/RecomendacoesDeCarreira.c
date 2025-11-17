// 2) Lista duplamente encadeada: recomendações de carreira.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>

// Tamanho máximo da string para a recomendação/etapa
#define TAMANHO_RECOMENDACAO 50

// Estrutura do Nó da Lista (Etapa de Carreira)
typedef struct NoEtapa {
    char recomendacao[TAMANHO_RECOMENDACAO]; // O dado (descrição da etapa)
    struct NoEtapa *anterior;               // Ponteiro para o nó anterior
    struct NoEtapa *proximo;                // Ponteiro para o nó próximo
} NoEtapa;

// Estrutura da Lista (Cabeça e Cauda da Lista)
typedef struct ListaCarreira {
    NoEtapa *cabeca; // Ponteiro para o primeiro nó
    NoEtapa *cauda;  // Ponteiro para o último nó
} ListaCarreira;

// Função para criar um novo nó de etapa
NoEtapa* criarNo(const char* nomeRecomendacao) {
    // Aloca memória para o novo nó
    NoEtapa* novoNo = (NoEtapa*)malloc(sizeof(NoEtapa));
    if (novoNo == NULL) {
        printf("Erro de alocação de memória.\n");
        exit(1);
    }
    // Copia o nome da recomendação
    strncpy(novoNo->recomendacao, nomeRecomendacao, TAMANHO_RECOMENDACAO - 1);
    novoNo->recomendacao[TAMANHO_RECOMENDACAO - 1] = '\0'; // Garantir terminação
    novoNo->anterior = NULL; 
    novoNo->proximo = NULL; 
    return novoNo;
}

// Função para limpar o buffer de entrada
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// 1. Inserir uma nova recomendação (insere no final da lista)
void inserirRecomendacao(ListaCarreira* lista, const char* nomeRecomendacao) {
    NoEtapa* novoNo = criarNo(nomeRecomendacao);

    // Caso 1: Lista Vazia
    if (lista->cabeca == NULL) {
        lista->cabeca = novoNo;
        lista->cauda = novoNo;
    } 
    // Caso 2: Lista Não Vazia (insere no final)
    else {
        // O próximo da cauda atual aponta para o novo nó
        lista->cauda->proximo = novoNo;
        // O anterior do novo nó aponta para a cauda atual
        novoNo->anterior = lista->cauda;
        // A cauda da lista passa a ser o novo nó
        lista->cauda = novoNo;
    }
    printf("\nEtapa de carreira '%s' inserida com sucesso.\n", nomeRecomendacao);
}

// 2. Encontrar e exibir uma recomendação específica
void encontrarRecomendacao(ListaCarreira* lista, const char* nomeBusca) {
    // Ponteiro que percorre a lista, começa na cabeça
    NoEtapa* atual = lista->cabeca;
    int posicao = 1;
    printf("\nBuscando etapa: '%s'...\n", nomeBusca);

    // Percorre a lista enquanto o ponteiro não for NULL
    while (atual != NULL) {
        // Compara a string da recomendação com a string de busca (ignorando maiúsculas/minúsculas)
        if (strcasecmp(atual->recomendacao, nomeBusca) == 0) {
            printf("Encontrada! Etapa: %s na posição %d.\n", atual->recomendacao, posicao);
            return; // Etapa encontrada, encerra a função
        }
        // Move para o próximo nó
        atual = atual->proximo;
        posicao++;
    }
    // Se saiu do loop sem retornar, a etapa não foi encontrada
    printf("Etapa '%s' não foi encontrada na lista.\n", nomeBusca);
}

// 3. Exibir a lista completa atualizada (Direta e Reversa)
// Exibição Direta (da cabeça para a cauda)
void exibirListaDireta(ListaCarreira* lista) {
    NoEtapa* atual = lista->cabeca;
    int contador = 1;
    printf("\nPlano de carreira (ordem direta):\n");

    if (atual == NULL) {
        printf("  (A lista de etapas está vazia.)\n");
        return;
    }
    while (atual != NULL) {
        printf("  %d. %s\n", contador, atual->recomendacao);
        atual = atual->proximo; // Navega para frente
        contador++;
    }
}

// Exibição Reversa (da cauda para a cabeça)
void exibirListaReversa(ListaCarreira* lista) {
    NoEtapa* atual = lista->cauda;
    int contador = 1;
    printf("\nPlano de carreira (ordem reversa):\n");
    
    if (atual == NULL) {
        printf("  (A lista de etapas está vazia.)\n");
        return;
    }
    while (atual != NULL) {
        printf("  %d. %s\n", contador, atual->recomendacao);
        atual = atual->anterior; // Navega para trás
        contador++;
    }
}

// Função para liberar a memória alocada
void liberarLista(ListaCarreira* lista) {
    NoEtapa* atual = lista->cabeca;
    NoEtapa* proximo;

    while (atual != NULL) {
        proximo = atual->proximo; // Guarda o ponteiro do próximo
        free(atual);              // Libera a memória do nó atual
        atual = proximo;          // Move para o próximo nó
    }
    lista->cabeca = NULL;
    lista->cauda = NULL;
}

// Função principal e menu
int main() {
    // Configura o console para usar UTF-8
    SetConsoleOutputCP(CP_UTF8); // Define o console para usar UTF-8
    SetConsoleCP(CP_UTF8); // Define a entrada do console para UTF-8

    // Configura a localidade para UTF-8 no sistema
    setlocale(LC_ALL, "pt_BR.UTF-8");

    // Inicializa a lista com a cabeça e cauda apontando para NULL (lista vazia)
    ListaCarreira lista;
    lista.cabeca = NULL; 
    lista.cauda = NULL;

    int escolha;
    char buffer[TAMANHO_RECOMENDACAO];

    do {
        // 4. Oferecer um menu ao usuário
        printf("\n==============================================\n");
        printf("   Plano de Carreira (Lista Duplamente Encadeada)\n");
        printf("==============================================\n");
        printf("1. Inserir nova recomendação/etapa\n");
        printf("2. Encontrar e exibir recomendação\n");
        printf("3. Exibir lista completa (Ordem Direta)\n");
        printf("4. Exibir lista completa (Ordem Reversa)\n");
        printf("0. Sair e encerrar o programa\n");
        printf("----------------------------------------------\n");
        printf("Escolha uma opção: ");
        
        // Lê a escolha do usuário
        if (scanf("%d", &escolha) != 1) {
            printf("\nOpção inválida. Por favor, digite um número.\n");
            limparBuffer(); // Limpa o buffer de entrada
            escolha = -1; // Define uma escolha inválida
            continue;
        }
        limparBuffer(); // Limpa o buffer após o scanf

        switch (escolha) {
            case 1:
                printf("\nDigite a nova etapa de carreira (máx. %d caracteres): ", TAMANHO_RECOMENDACAO - 1);
                // Lê a string com espaços
                if (fgets(buffer, TAMANHO_RECOMENDACAO, stdin) != NULL) {
                    // Remove o '\n' que o fgets adiciona
                    buffer[strcspn(buffer, "\n")] = 0; 
                    if (strlen(buffer) > 0) {
                        inserirRecomendacao(&lista, buffer);
                    } else {
                        printf("Etapa não pode ser vazia.\n");
                    }
                }
                break;
            case 2:
                if (lista.cabeca == NULL) {
                    printf("\nA lista está vazia. Insira etapas primeiro.\n");
                    break;
                }
                printf("\nDigite a etapa que deseja buscar: ");
                if (fgets(buffer, TAMANHO_RECOMENDACAO, stdin) != NULL) {
                    buffer[strcspn(buffer, "\n")] = 0;
                    if (strlen(buffer) > 0) {
                        encontrarRecomendacao(&lista, buffer);
                    } else {
                        printf("A busca não pode ser vazia.\n");
                    }
                }
                break;
            case 3:
                exibirListaDireta(&lista);
                break;
            case 4:
                exibirListaReversa(&lista);
                break;
            case 0:
                printf("\nEncerrando o programa. Memória da lista liberada.\n");
                liberarLista(&lista);
                break;
            default:
                printf("\nOpção inválida. Por favor, escolha uma opção entre 0 e 4.\n");
                break;
        }
    } while (escolha != 0);

    return 0;

}
