// 3) Lista circular: aprendizado contínuo.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>

// Tamanho máximo da string para o curso
#define TAMANHO_CURSO 80

// Estrutura do Nó da Lista (Curso)
typedef struct NoCurso {
    char nomeCurso[TAMANHO_CURSO]; // O dado (nome do curso)
    struct NoCurso *proximo;       // Ponteiro para o próximo nó
} NoCurso;

// Estrutura da Lista Circular (Gerencia o ciclo)
typedef struct CicloAprendizado {
    NoCurso *ultimo;     // Ponteiro que aponta para o último nó da lista
    NoCurso *cursoAtual; // Ponteiro que representa onde o usuário está no ciclo
} CicloAprendizado;

// Função para criar um novo nó de curso
NoCurso* criarNo(const char* nome) {
    // Aloca memória para o novo nó
    NoCurso* novoNo = (NoCurso*)malloc(sizeof(NoCurso));
    if (novoNo == NULL) {
        printf("Erro de alocação de memória.\n");
        exit(1);
    }
    // Copia o nome do curso
    strncpy(novoNo->nomeCurso, nome, TAMANHO_CURSO - 1);
    novoNo->nomeCurso[TAMANHO_CURSO - 1] = '\0'; // Garantir terminação
    novoNo->proximo = NULL; 
    return novoNo;
}

// Função para limpar o buffer de entrada
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// 1. Inserir Curso (Insere no final da lista)
void inserirCurso(CicloAprendizado* ciclo, const char* nome) {
    NoCurso* novoNo = criarNo(nome);
    // Caso 1: Lista Vazia
    if (ciclo->ultimo == NULL) {
        // O nó aponta para si mesmo
        novoNo->proximo = novoNo;
        // O último e o curso atual são este novo nó
        ciclo->ultimo = novoNo;
        ciclo->cursoAtual = novoNo;
    } 
    // Caso 2: Lista Não Vazia
    else {
        // O 'proximo' do novo nó aponta para o 'proximo' do antigo último (que é o primeiro)
        novoNo->proximo = ciclo->ultimo->proximo; 
        // O 'proximo' do antigo último aponta para o novo nó
        ciclo->ultimo->proximo = novoNo;
        // O novo nó se torna o último
        ciclo->ultimo = novoNo;
    }
    printf("\nCurso '%s' inserido no ciclo de aprendizado.\n", nome);
}

// 2. Exibir ciclo completo
void exibirCicloCompleto(CicloAprendizado* ciclo) {
    // Lista vazia
    if (ciclo->ultimo == NULL) {
        printf("\nO ciclo de aprendizado está vazio.\n");
        return;
    }
    // Começa pelo primeiro curso (o próximo do último)
    NoCurso* atual = ciclo->ultimo->proximo;
    int contador = 1;
    printf("\nCiclo completo de aprendizado:\n");
    
    // Percorre a lista até voltar ao primeiro nó
    do {
        // Marca o curso atual com um asterisco
        if (atual == ciclo->cursoAtual) {
            printf("  %d. Estudando %s\n", contador, atual->nomeCurso);
        } else {
            printf("  %d. %s\n", contador, atual->nomeCurso);
        }
        atual = atual->proximo; // Avança para o próximo
        contador++;
    } while (atual != ciclo->ultimo->proximo); // Continua até voltar ao início (o próximo do último)
}

// 3. Simular avanço cíclico
void simularAvanco(CicloAprendizado* ciclo) {
    if (ciclo->ultimo == NULL) {
        printf("\nO ciclo está vazio. Insira cursos primeiro.\n");
        return;
    }
    printf("\nAvançando no ciclo de aprendizado...\n");
    
    // O curso atual avança para o próximo nó
    ciclo->cursoAtual = ciclo->cursoAtual->proximo;
    // Se o curso atual for o primeiro novamente, ele concluiu uma volta
    if (ciclo->cursoAtual == ciclo->ultimo->proximo) {
        printf("Fim de um ciclo! Retornando ao primeiro curso.\n");
    }
    printf("  Agora você está estudando: %s.\n", ciclo->cursoAtual->nomeCurso);
}

// Função para liberar a memória alocada
void liberarCiclo(CicloAprendizado* ciclo) {
    if (ciclo->ultimo == NULL) return;
    NoCurso* primeiro = ciclo->ultimo->proximo;
    NoCurso* atual = primeiro;
    NoCurso* proximo;

    // Remove a ligação circular para evitar loop infinito na liberação
    ciclo->ultimo->proximo = NULL; 
    while (atual != NULL) {
        proximo = atual->proximo; // Guarda o ponteiro do próximo
        free(atual);              // Libera a memória do nó atual
        atual = proximo;          // Move para o próximo nó
    }
    ciclo->ultimo = NULL;
    ciclo->cursoAtual = NULL;
}

int main() {
    // Configura o console para usar UTF-8
    SetConsoleOutputCP(CP_UTF8); // Define o console para usar UTF-8
    SetConsoleCP(CP_UTF8); // Define a entrada do console para UTF-8

    // Configura a localidade para UTF-8 no sistema
    setlocale(LC_ALL, "pt_BR.UTF-8");

    // Inicializa o ciclo com ponteiros NULL (lista vazia)
    CicloAprendizado ciclo;
    ciclo.ultimo = NULL; 
    ciclo.cursoAtual = NULL;

    int escolha;
    char buffer[TAMANHO_CURSO];

    // Adicionando alguns cursos iniciais para facilitar o teste
    inserirCurso(&ciclo, "Fundamentos de C");
    inserirCurso(&ciclo, "Estruturas de Dados");
    inserirCurso(&ciclo, "Algoritmos Avançados");

    do {
        // Oferecer um menu ao usuário
        printf("\n==============================================\n");
        printf("     Ciclo de Aprendizado (Lista Circular)\n");
        printf("==============================================\n");
        printf("1. Inserir novo curso\n");
        printf("2. Exibir ciclo completo (marcando o atual)\n");
        printf("3. Simular avanço para o próximo curso\n");
        printf("0. Sair e encerrar o programa\n");
        printf("----------------------------------------------\n");
        // Exibe o curso atual antes do menu principal
        if (ciclo.cursoAtual != NULL) {
            printf("Curso Atual: %s\n", ciclo.cursoAtual->nomeCurso);
        }
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
                printf("\nDigite o nome do novo curso (máx. %d caracteres): ", TAMANHO_CURSO - 1);
                if (fgets(buffer, TAMANHO_CURSO, stdin) != NULL) {
                    buffer[strcspn(buffer, "\n")] = 0; 
                    if (strlen(buffer) > 0) {
                        inserirCurso(&ciclo, buffer);
                    } else {
                        printf("O nome do curso não pode ser vazio.\n");
                    }
                }
                break;
            case 2:
                exibirCicloCompleto(&ciclo);
                break;
            case 3:
                simularAvanco(&ciclo);
                break;
            case 0:
                printf("\nEncerrando o programa. Memória do ciclo liberada.\n");
                liberarCiclo(&ciclo); // Libera toda a memória antes de sair
                break;
            default:
                printf("\nOpção inválida. Por favor, escolha uma opção entre 0 e 3.\n");
                break;
        }
    } while (escolha != 0);

    return 0;

}
