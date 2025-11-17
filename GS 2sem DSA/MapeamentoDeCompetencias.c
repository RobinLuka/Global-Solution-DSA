// 1) Lista simplesmente encadeada: mapeamento de competências.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>

// Tamanho máximo da string para a habilidade
#define TAMANHO_HABILIDADE 50

// Estrutura do Nó da Lista (Elemento da Habilidade)
typedef struct NoHabilidade {
    char habilidade[TAMANHO_HABILIDADE]; // O dado (nome da habilidade)
    struct NoHabilidade *proximo;        // O ponteiro (próximo nó)
} NoHabilidade;

// Estrutura da Lista (Cabeça da Lista)
typedef struct ListaCompetencias {
    NoHabilidade *cabeca; // Ponteiro para o primeiro nó
} ListaCompetencias;

// Função para criar um novo nó de habilidade
NoHabilidade* criarNo(const char* nomeHabilidade) {
    // Aloca memória para o novo nó
    NoHabilidade* novoNo = (NoHabilidade*)malloc(sizeof(NoHabilidade));
    if (novoNo == NULL) {
        printf("Erro de alocação de memória.\n");
        exit(1);
    }
    // Copia o nome da habilidade para o nó
    strncpy(novoNo->habilidade, nomeHabilidade, TAMANHO_HABILIDADE - 1);
    novoNo->habilidade[TAMANHO_HABILIDADE - 1] = '\0'; // Garantir terminação
    novoNo->proximo = NULL; // O próximo é inicialmente NULL
    return novoNo;
}

// 1. Inserir uma nova habilidade (insere no início da lista)
void inserirHabilidade(ListaCompetencias* lista, const char* nomeHabilidade) {
    NoHabilidade* novoNo = criarNo(nomeHabilidade);
    // O próximo do novo nó aponta para o nó que era a cabeça atual
    novoNo->proximo = lista->cabeca;
    // A cabeça da lista passa a ser o novo nó
    lista->cabeca = novoNo;
    printf("\nHabilidade '%s' inserida com sucesso.\n", nomeHabilidade);
}

// 2. Encontrar e exibir uma habilidade específica
void encontrarHabilidade(ListaCompetencias* lista, const char* nomeBusca) {
    // Ponteiro que percorre a lista, começa na cabeça
    NoHabilidade* atual = lista->cabeca;
    int posicao = 1;
    printf("\nBuscando habilidade: '%s'...\n", nomeBusca);

    // Percorre a lista enquanto o ponteiro não for NULL
    while (atual != NULL) {
        // Compara a string da habilidade com a string de busca (ignorando maiúsculas/minúsculas)
        if (strcasecmp(atual->habilidade, nomeBusca) == 0) {
            printf(" Encontrada! Habilidade: **%s** na posição **%d**.\n", atual->habilidade, posicao);
            return; // Habilidade encontrada, encerra a função
        }
        // Move para o próximo nó
        atual = atual->proximo;
        posicao++;
    }

    // Se saiu do loop sem retornar, a habilidade não foi encontrada
    printf("Habilidade '%s' não foi encontrada na lista.\n", nomeBusca);
}

// 3. Exibir a lista completa atualizada
void exibirLista(ListaCompetencias* lista) {
    // Ponteiro que percorre a lista, começa na cabeça
    NoHabilidade* atual = lista->cabeca;
    int contador = 1;
    printf("\n**LISTA ATUAL DE COMPETÊNCIAS**:\n");
    
    // Verifica se a lista está vazia
    if (atual == NULL) {
        printf("  (A lista de competências está vazia.)\n");
        return;
    }
    // Percorre e exibe cada nó
    while (atual != NULL) {
        printf("  %d. %s\n", contador, atual->habilidade);
        atual = atual->proximo; // Vai para o próximo nó
        contador++;
    }
}

// Função para liberar a memória alocada pela lista
void liberarLista(ListaCompetencias* lista) {
    NoHabilidade* atual = lista->cabeca;
    NoHabilidade* proximo;

    while (atual != NULL) {
        proximo = atual->proximo; // Guarda o ponteiro do próximo antes de liberar o atual
        free(atual);              // Libera a memória do nó atual
        atual = proximo;          // Move para o próximo nó
    }
    lista->cabeca = NULL; // Garante que a lista está vazia
}

// Função principal e menu
int main() {
    // Configura o console para usar UTF-8
    SetConsoleOutputCP(CP_UTF8); // Define o console para usar UTF-8
    SetConsoleCP(CP_UTF8); // Define a entrada do console para UTF-8

    // Configura a localidade para UTF-8 no sistema
    setlocale(LC_ALL, "pt_BR.UTF-8");

    // Inicializa a lista com a cabeça apontando para NULL (lista vazia)
    ListaCompetencias lista;
    lista.cabeca = NULL; 

    int escolha;
    char buffer[TAMANHO_HABILIDADE];

    do {
        // 4. Oferecer um menu ao usuário
        printf("\n==============================================\n");
        printf("       Mapeamento de Competências (Lista Encadeada)\n");
        printf("==============================================\n");
        printf("1. Inserir nova habilidade\n");
        printf("2. Encontrar e exibir habilidade\n");
        printf("3. Exibir lista completa\n");
        printf("0. Sair e encerrar o programa\n");
        printf("----------------------------------------------\n");
        printf("Escolha uma opção: ");
        
        // Lê a escolha do usuário
        if (scanf("%d", &escolha) != 1) {
             // Lida com entrada não numérica
            printf("\nOpção inválida. Por favor, digite um número.\n");
            // Limpa o buffer de entrada para evitar loop infinito
            while (getchar() != '\n');
            escolha = -1; // Define uma escolha inválida
            continue;
        }
        // Limpa o buffer para o próximo `scanf` ou `fgets`
        while (getchar() != '\n'); 

        switch (escolha) {
            case 1:
                printf("\nDigite a nova habilidade (máx. %d caracteres): ", TAMANHO_HABILIDADE - 1);
                // Lê a string com espaços
                if (fgets(buffer, TAMANHO_HABILIDADE, stdin) != NULL) {
                    // Remove o '\n' que o fgets adiciona
                    buffer[strcspn(buffer, "\n")] = 0; 
                    if (strlen(buffer) > 0) {
                        inserirHabilidade(&lista, buffer);
                    } else {
                        printf("Habilidade não pode ser vazia.\n");
                    }
                }
                break;
            case 2:
                if (lista.cabeca == NULL) {
                    printf("\nA lista está vazia. Insira habilidades primeiro.\n");
                    break;
                }
                printf("\nDigite a habilidade que deseja buscar: ");
                if (fgets(buffer, TAMANHO_HABILIDADE, stdin) != NULL) {
                    buffer[strcspn(buffer, "\n")] = 0;
                    if (strlen(buffer) > 0) {
                        encontrarHabilidade(&lista, buffer);
                    } else {
                        printf("A busca não pode ser vazia.\n");
                    }
                }
                break;
            case 3:
                exibirLista(&lista);
                break;
            case 0:
                printf("\nEncerrando o programa. Memória da lista liberada.\n");
                liberarLista(&lista); // Libera toda a memória antes de sair
                break;
            default:
                printf("\nOpção inválida. Por favor, escolha uma opção entre 0 e 3.\n");
                break;
        }
    } while (escolha != 0);

    return 0;

}
