#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_CACHE 4


typedef struct {
    int dado;
    int ultima_utilizacao;
} BlocoCache;

// Função para imprimir o estado da cache
void printCache(BlocoCache cache[], int tempoAtual, int mostrarUltimaUtilizacao) {
    for (int i = 0; i < TAMANHO_CACHE; i++) {
        if (cache[i].dado != -1) {
            if (mostrarUltimaUtilizacao) {
                printf("Cache[%d]: %d (Nao usado ha: %d)\n", i, cache[i].dado, tempoAtual - cache[i].ultima_utilizacao);
            } else {
                printf("Cache[%d]: %d\n", i, cache[i].dado);
            }
        } else {
            printf("Cache[%d]: Vazio\n", i);
        }
    }
    printf("\n");
}

// Simulação da política FIFO
void simularFIFO(int dados[], int tamanhoDados) {
    BlocoCache cache[TAMANHO_CACHE];
    for (int i = 0; i < TAMANHO_CACHE; i++) {
        cache[i].dado = -1; // Inicializa a cache como vazia
    }

    int indice = 0;
    printf("\nPolítica FIFO:\n");
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
        printCache(cache, 0, 0); // Passa 0 para não mostrar "Não usado há"
    }
}

// Simulação da política LRU
void simularLRU(int dados[], int tamanhoDados) {
    BlocoCache cache[TAMANHO_CACHE];
    for (int i = 0; i < TAMANHO_CACHE; i++) {
        cache[i].dado = -1; // Inicializa a cache como vazia
        cache[i].ultima_utilizacao = 0;
    }

    printf("\nPolítica LRU:\n");
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
        printf("Ciclo: %d\n", i + 1);
        printCache(cache, tempo, 1);
    }
}

void simularRANDOM(int dados[], int tamanhoDados) {
    BlocoCache cache[TAMANHO_CACHE];
    for (int i = 0; i < TAMANHO_CACHE; i++) {
        cache[i].dado = -1; // Inicializa a cache como vazia
    }

    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    printf("\nPolítica RANDOM:\n");
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
        printf("Ciclo: %d\n", i + 1);
        printCache(cache, 0, 0); // Passa 0 para não mostrar "Não usado há"
    }
}

int main() {

    int dados[] = {1, 2, 3, 4, 1, 2, 5, 1, 6, 2, 3, 2, 7, 2, 5, 7, 1};
    int tamanhoDados = sizeof(dados) / sizeof(dados[0]);

    //simularFIFO(dados, tamanhoDados);
    //simularLRU(dados, tamanhoDados);
    simularRANDOM(dados, tamanhoDados);

    return 0;
}
