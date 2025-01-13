#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_CACHE 4

typedef struct {
    int dado;
    int ultima_utilizacao; // Para LRU
    int frequencia;        // Para LFU
} BlocoCache;

// Função para imprimir o estado da cache
void printCache(BlocoCache cache[], int tempoAtual, int mostrarUltimaUtilizacao, int mostrarFrequencia) {
    for (int i = 0; i < TAMANHO_CACHE; i++) {
        if (cache[i].dado != -1) {
            if (mostrarUltimaUtilizacao) {
                printf("Cache[%d]: %d (Nao usado ha: %d)\n", i, cache[i].dado, tempoAtual - cache[i].ultima_utilizacao);
            } else if (mostrarFrequencia) {
                printf("Cache[%d]: %d (Frequencia: %d)\n", i, cache[i].dado, cache[i].frequencia);
            } else {
                printf("Cache[%d]: %d\n", i, cache[i].dado);
            }
        } else {
            printf("Cache[%d]: Vazio\n", i);
        }
    }
    printf("\n");
}

// Simulação da Politica FIFO
void simularFIFO(int dados[], int tamanhoDados) {
    BlocoCache cache[TAMANHO_CACHE];
    for (int i = 0; i < TAMANHO_CACHE; i++) {
        cache[i].dado = -1; // Inicializa a cache como vazia
    }

    int indice = 0;
    printf("\nPolitica FIFO:\n");
    for (int i = 0; i < tamanhoDados; i++) {
        int encontrado = 0;
        for (int j = 0; j < TAMANHO_CACHE; j++) {
            if (cache[j].dado == dados[i]) {
                encontrado = 1;
                break;
            }
        }
        if (!encontrado) {
            cache[indice].dado = dados[i];
            indice = (indice + 1) % TAMANHO_CACHE; // Substitui o próximo bloco na fila
        }
        printCache(cache, 0, 0, 0); // Passa 0 para não mostrar "Não usado há" ou "Frequencia"
    }
}

// Simulação da Politica LRU
void simularLRU(int dados[], int tamanhoDados) {
    BlocoCache cache[TAMANHO_CACHE];
    for (int i = 0; i < TAMANHO_CACHE; i++) {
        cache[i].dado = -1; 
        cache[i].ultima_utilizacao = 0;
    }

    printf("\nPolitica LRU:\n");
    int tempo = 0;
    for (int i = 0; i < tamanhoDados; i++) {
        tempo++;
        int encontrado = 0, indiceLRU = 0, menorTempo = tempo;

        for (int j = 0; j < TAMANHO_CACHE; j++) {
            if (cache[j].dado == dados[i]) {
                encontrado = 1;
                cache[j].ultima_utilizacao = tempo;
                break;
            }
            if (cache[j].dado == -1 || cache[j].ultima_utilizacao < menorTempo) {
                menorTempo = cache[j].ultima_utilizacao;
                indiceLRU = j;
            }
        }

        if (!encontrado) {
            cache[indiceLRU].dado = dados[i];
            cache[indiceLRU].ultima_utilizacao = tempo;
        }
        printCache(cache, tempo, 1, 0); // Mostra "Não usado há" e não a frequencia
    }
}

// Simulação da Politica RANDOM
void simularRANDOM(int dados[], int tamanhoDados) {
    BlocoCache cache[TAMANHO_CACHE];
    for (int i = 0; i < TAMANHO_CACHE; i++) {
        cache[i].dado = -1; // Inicializa a cache como vazia
    }

    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    printf("\nPolitica RANDOM:\n");
    for (int i = 0; i < tamanhoDados; i++) {
        int encontrado = 0;
        for (int j = 0; j < TAMANHO_CACHE; j++) {
            if (cache[j].dado == dados[i]) {
                encontrado = 1;
                break;
            }
        }
        if (!encontrado) {
            int indiceAleatorio = rand() % TAMANHO_CACHE; // Seleciona um índice aleatório para substituição
            cache[indiceAleatorio].dado = dados[i];
        }
        printCache(cache, 0, 0, 0); // Passa 0 para não mostrar "Não usado há" ou "Frequencia"
    }
}

// Simulação da Politica LFU
void simularLFU(int dados[], int tamanhoDados) {
    BlocoCache cache[TAMANHO_CACHE];
    for (int i = 0; i < TAMANHO_CACHE; i++) {
        cache[i].dado = -1; // Inicializa a cache como vazia
        cache[i].frequencia = 0;
    }

    printf("\nPolitica LFU:\n");
    for (int i = 0; i < tamanhoDados; i++) {
        int encontrado = 0;
        int indiceLFU = 0;
        int menorFrequencia = cache[0].frequencia;

        // Verifica se o dado já está na cache
        for (int j = 0; j < TAMANHO_CACHE; j++) {
            if (cache[j].dado == dados[i]) {
                encontrado = 1;
                cache[j].frequencia++; // Incrementa a frequencia de uso
                break;
            }
        }

        // Se não encontrado, precisamos adicionar/substituir
        if (!encontrado) {
            // Encontra o índice com a menor frequencia de uso
            for (int j = 0; j < TAMANHO_CACHE; j++) {
                if (cache[j].dado == -1 || cache[j].frequencia < menorFrequencia) {
                    menorFrequencia = cache[j].frequencia;
                    indiceLFU = j;
                }
            }

            // Substitui o bloco LFU ou preenche o espaco vazio
            cache[indiceLFU].dado = dados[i];
            cache[indiceLFU].frequencia = 1; // Reseta a frequencia de uso
        }
        printCache(cache, 0, 0, 1); // Não mostrar "Não usado há", mas mostrar "Frequencia"
    }
}

int main() {
    int dados[] = {1, 2, 3, 4, 1, 5, 1, 6, 2, 3, 2, 7, 5, 1};
    int tamanhoDados = sizeof(dados) / sizeof(dados[0]);
    int escolha;

    printf("\nEscolha a politica de substituicao:\n");
    printf("1 - FIFO\n2 - LRU\n3 - LFU\n4 - RANDOM\n");
    printf("Digite sua escolha: ");
    scanf("%d", &escolha);

    switch (escolha) {
        case 1:
            simularFIFO(dados, tamanhoDados);
            break;
        case 2:
            simularLRU(dados, tamanhoDados);
            break;
        case 3:
            simularLFU(dados, tamanhoDados);
            break;
        case 4:
            simularRANDOM(dados, tamanhoDados);
            break;
        default:
            printf("Escolha inválida!\n");
    }

    return 0;
}
