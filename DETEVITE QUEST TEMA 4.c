#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SUSPEITOS 10
#define HASH_SIZE 10

// ==================== Estruturas ====================

// Nó da Árvore Binária - Salas da Mansão
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// Nó da BST - Pistas coletadas
typedef struct PistaNode {
    char pista[100];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;

// Entrada da Tabela Hash (Pista -> Suspeito)
typedef struct {
    char pista[100];
    char suspeito[50];
    int ocupado; // 0 = livre, 1 = ocupado
} HashEntry;

// ==================== Funções de Árvore Binária ====================

Sala* criarSala(char* nome, char* pista) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// ==================== Funções de BST ====================

PistaNode* inserirPista(PistaNode* raiz, char* pista) {
    if (!raiz) {
        PistaNode* novo = (PistaNode*)malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esquerda = novo->direita = NULL;
        return novo;
    }
    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);
    return raiz;
}

void exibirPistas(PistaNode* raiz) {
    if (!raiz) return;
    exibirPistas(raiz->esquerda);
    printf("- %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

// ==================== Funções de Hash ====================

int hashFunc(char* chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++)
        soma += chave[i];
    return soma % HASH_SIZE;
}

void inserirNaHash(HashEntry tabela[], char* pista, char* suspeito) {
    int idx = hashFunc(pista);
    while (tabela[idx].ocupado) {
        idx = (idx + 1) % HASH_SIZE;
    }
    strcpy(tabela[idx].pista, pista);
    strcpy(tabela[idx].suspeito, suspeito);
    tabela[idx].ocupado = 1;
}

char* encontrarSuspeito(HashEntry tabela[], char* pista) {
    int idx = hashFunc(pista);
    int start = idx;
    while (tabela[idx].ocupado) {
        if (strcmp(tabela[idx].pista, pista) == 0)
            return tabela[idx].suspeito;
        idx = (idx + 1) % HASH_SIZE;
        if (idx == start) break;
    }
    return NULL;
}

// ==================== Função de Exploração ====================

void explorarSalas(Sala* sala, PistaNode** bst, HashEntry tabela[]) {
    if (!sala) return;
    printf("\nVocê está na sala: %s\n", sala->nome);
    if (strlen(sala->pista) > 0) {
        printf("Você encontrou uma pista: %s\n", sala->pista);
        *bst = inserirPista(*bst, sala->pista);
    }

    printf("Escolha o caminho: [e] esquerda | [d] direita | [s] sair: ");
    char escolha;
    scanf(" %c", &escolha);
    if (escolha == 'e')
        explorarSalas(sala->esquerda, bst, tabela);
    else if (escolha == 'd')
        explorarSalas(sala->direita, bst, tabela);
}

// ==================== Função de Verificação ====================

void verificarSuspeitoFinal(PistaNode* bst, HashEntry tabela[]) {
    char acusado[50];
    printf("\nDigite o nome do suspeito que deseja acusar: ");
    scanf("%s", acusado);

    // Contar pistas que apontam para o suspeito
    int contador = 0;

    // Percorrer BST de pistas coletadas
    PistaNode* stack[100]; // Pilha para percurso iterativo
    int topo = -1;
    PistaNode* atual = bst;

    while (atual || topo != -1) {
        while (atual) {
            stack[++topo] = atual;
            atual = atual->esquerda;
        }
        atual = stack[topo--];
        char* suspeito = encontrarSuspeito(tabela, atual->pista);
        if (suspeito && strcmp(suspeito, acusado) == 0)
            contador++;
        atual = atual->direita;
    }

    if (contador >= 2)
        printf("\nAcusação correta! O suspeito %s é o culpado!\n", acusado);
    else
        printf("\nAcusação incorreta. O suspeito %s não possui pistas suficientes.\n", acusado);
}

// ==================== Função Main ====================

int main() {
    // Criar salas da mansão
    Sala* hall = criarSala("Hall de Entrada", "Pegada de sapato");
    Sala* cozinha = criarSala("Cozinha", "Luvas encontradas");
    Sala* salaEstar = criarSala("Sala de Estar", "Carta rasgada");
    Sala* jardim = criarSala("Jardim", "Pegada de sapato");
    Sala* biblioteca = criarSala("Biblioteca", "Chave escondida");

    // Montar árvore binária
    hall->esquerda = cozinha;
    hall->direita = salaEstar;
    cozinha->esquerda = jardim;
    salaEstar->direita = biblioteca;

    // Inicializar BST de pistas
    PistaNode* bst = NULL;

    // Inicializar tabela hash
    HashEntry tabela[HASH_SIZE];
    for (int i = 0; i < HASH_SIZE; i++) tabela[i].ocupado = 0;

    // Associar pistas a suspeitos
    inserirNaHash(tabela, "Pegada de sapato", "Sr. Black");
    inserirNaHash(tabela, "Luvas encontradas", "Sra. White");
    inserirNaHash(tabela, "Carta rasgada", "Sr. Green");
    inserirNaHash(tabela, "Chave escondida", "Sra. Pink");

    // Exploração
    explorarSalas(hall, &bst, tabela);

    // Exibir pistas coletadas
    printf("\nPistas coletadas (em ordem alfabética):\n");
    exibirPistas(bst);

    // Julgamento final
    verificarSuspeitoFinal(bst, tabela);

    return 0;
}
