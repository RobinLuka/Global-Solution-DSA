#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>

// Tamanho máximo da pilha (número máximo de ações)
#define CAPACIDADE_MAXIMA 10
// Tamanho máximo da string para a ação
#define TAMANHO_ACAO 50

// Estrutura da Pilha de Histórico
typedef struct PilhaHistorico {
    char acoes[CAPACIDADE_MAXIMA][TAMANHO_ACAO]; // Array de strings para armazenar as ações
    int topo; // Variável que indica o índice do topo da pilha (último elemento inserido)
} PilhaHistorico;

// Função para inicializar a pilha
void inicializarPilha(PilhaHistorico* pilha) {
    // Quando a pilha está vazia, o topo é -1
    pilha->topo = -1;
    printf("\nPilha de histórico inicializada. Topo: %d.\n", pilha->topo);
}

// Função para verificar se a pilha está vazia
int estaVazia(PilhaHistorico* pilha) {
    return (pilha->topo == -1);
}

// Função para verificar se a pilha está cheia
int estaCheia(PilhaHistorico* pilha) {
    return (pilha->topo == CAPACIDADE_MAXIMA - 1);
}

// Função para limpar o buffer de entrada
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// 1. Empilhar novas ações (Push)
void empilharAcao(PilhaHistorico* pilha, const char* novaAcao) {
    // Verifica se a pilha está cheia
    if (estaCheia(pilha)) {
        printf("\nErro: A pilha de histórico está cheia. Não é possível empilhar mais ações.\n");
        return;
    }
    // Incrementa o topo
    pilha->topo++;
    
    // Copia a nova ação para a posição do novo topo
    strncpy(pilha->acoes[pilha->topo], novaAcao, TAMANHO_ACAO - 1);
    pilha->acoes[pilha->topo][TAMANHO_ACAO - 1] = '\0';

    printf("\nAção empilhada: '%s'. (Topo: %d)\n", novaAcao, pilha->topo);
}

// 2. Desempilhar a última ação (Pop) - Simula o "desfazer"
void desempilharAcao(PilhaHistorico* pilha) {
    // Verifica se a pilha está vazia
    if (estaVazia(pilha)) {
        printf("\nErro: A pilha de histórico está vazia. Não há ações para desfazer.\n");
        return;
    }
    // Ação que será desempilhada (a ação no topo atual)
    char acaoRemovida[TAMANHO_ACAO];
    strncpy(acaoRemovida, pilha->acoes[pilha->topo], TAMANHO_ACAO);

    // Decrementa o topo (a ação logicamente é removida/ignorada)
    pilha->topo--;

    printf("\nAção desempilhada (Desfeita): %s.\n", acaoRemovida);
    printf("  Novo topo: %d.\n", pilha->topo);
}

// 3. Mostrar o histórico atual (do topo para a base)
void mostrarHistorico(PilhaHistorico* pilha) {
    if (estaVazia(pilha)) {
        printf("\nHistórico atual:\n  (A pilha está vazia. Comece a registrar ações!)\n");
        return;
    }
    printf("\nHistórico atual (Topo para Base):\n");
    // Percorre a pilha do topo (última ação) até a base (primeira ação)
    for (int i = pilha->topo; i >= 0; i--) {
        if (i == pilha->topo) {
            printf("  %d. Topo %s\n", i + 1, pilha->acoes[i]); // Marca o topo
        } else {
            printf("  %d. %s\n", i + 1, pilha->acoes[i]);
        }
    }
}

int main() {
    // Configura o console para usar UTF-8
    SetConsoleOutputCP(CP_UTF8); // Define o console para usar UTF-8
    SetConsoleCP(CP_UTF8); // Define a entrada do console para UTF-8

    // Configura a localidade para UTF-8 no sistema
    setlocale(LC_ALL, "pt_BR.UTF-8");

    // Inicializa a pilha
    PilhaHistorico historico;
    inicializarPilha(&historico); 

    int escolha;
    char buffer[TAMANHO_ACAO];

    do {
        // 4. Oferecer um menu ao usuário
        printf("\n==========================================\n");
        printf("     Pilha: Histórico de Estudos\n");
        printf("=========================================\n");
        printf("1. Empilhar nova ação (Registrar Estudo)\n");
        printf("2. Desempilhar última ação (Desfazer)\n");
        printf("3. Mostrar histórico atual\n");
        printf("0. Sair e encerrar o programa\n");
        printf("----------------------------------------------\n");
        printf("Escolha uma opção: ");
        // Lê a escolha do usuário
        if (scanf("%d", &escolha) != 1) {
            printf("\nOpção inválida. Por favor, digite um número.\n");
            limparBuffer(); // Limpa o buffer de entrada
            escolha = -1; 
            continue;
        }
        limparBuffer(); // Limpa o buffer após o scanf

        switch (escolha) {
            case 1:
                printf("\nDigite a ação de estudo/revisão (máx. %d caracteres): ", TAMANHO_ACAO - 1);
                if (fgets(buffer, TAMANHO_ACAO, stdin) != NULL) {
                    // Remove o '\n' que o fgets adiciona
                    buffer[strcspn(buffer, "\n")] = 0; 
                    if (strlen(buffer) > 0) {
                        empilharAcao(&historico, buffer);
                    } else {
                        printf("A ação não pode ser vazia.\n");
                    }
                }
                break;
            case 2:
                desempilharAcao(&historico);
                break;
            case 3:
                mostrarHistorico(&historico);
                break;
            case 0:
                printf("\nEncerrando o programa. Histórico de estudos finalizado.\n");
                break;
            default:
                printf("\nOpção inválida. Por favor, escolha uma opção entre 0 e 3.\n");
                break;
        }
    } while (escolha != 0);

    return 0;
}