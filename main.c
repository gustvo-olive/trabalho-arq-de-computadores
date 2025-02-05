#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_CACHE 10

// Variáveis globais para as métricas
int total_hits = 0;
int total_misses = 0;

typedef struct {
    int dado;
    int ultima_utilizacao; // Para LRU
    int frequencia;        // Para LFU
} BlocoCache;

// Função para imprimir a cache
void printCache(BlocoCache cache[], int tempoAtual, int mostrarUltimaUtilizacao, int mostrarFrequencia, int indiceSubstituido) {
    for (int i = 0; i < TAMANHO_CACHE; i++) {
        if (cache[i].dado != -1) {
            if (i == indiceSubstituido) {
                printf("\033[31m"); // Vermelho para dados substituídos
                printf("Cache[%d]: %d ", i, cache[i].dado);
                printf("\033[0m"); // Resetar cor
            } else {
                printf("Cache[%d]: %d ", i, cache[i].dado);
            }

            if (mostrarUltimaUtilizacao) {
                printf("(Nao usado ha: %d)", tempoAtual - cache[i].ultima_utilizacao);
            } else if (mostrarFrequencia) {
                printf("(Frequencia: %d)", cache[i].frequencia);
            }
            printf("\n");
        } else {
            printf("Cache[%d]: Vazio\n", i);
        }
    }
    printf("\n");
}

// Função para exibir métricas ao final de cada simulação
void exibirMetricas() {
    int total_acessos = total_hits + total_misses;
    float hit_rate = (total_acessos > 0) ? ((float)total_hits / total_acessos) * 100 : 0;
    float miss_rate = (total_acessos > 0) ? ((float)total_misses / total_acessos) * 100 : 0;

    printf("\nMetricas de Desempenho:\n");
    printf("Total de acessos: %d\n", total_acessos);
    printf("Total de hits (acertos): %d\n", total_hits);
    printf("Total de misses (falhas): %d\n", total_misses);
    printf("Hit Rate: %.2f%%\n", hit_rate);
    printf("Miss Rate: %.2f%%\n", miss_rate);
}

// 1. Simulação da Política FIFO
void simularFIFO(int dados[], int tamanhoDados) {
    BlocoCache cache[TAMANHO_CACHE];
    for (int i = 0; i < TAMANHO_CACHE; i++) {
        cache[i].dado = -1;
    }

    int indice = 0;
    total_hits = total_misses = 0;

    printf("\nPolítica FIFO:\n");
    for (int i = 0; i < tamanhoDados; i++) {
        int encontrado = 0;
        for (int j = 0; j < TAMANHO_CACHE; j++) {
            if (cache[j].dado == dados[i]) {
                encontrado = 1;
                total_hits++; // Contabiliza acerto
                break;
            }
        }

        int indiceSubstituido = -1;
        if (!encontrado) {
            total_misses++; // Contabiliza falha
            indiceSubstituido = indice;
            cache[indice].dado = dados[i];
            indice = (indice + 1) % TAMANHO_CACHE;
        }

        printf("Dado: %d\n", dados[i]);
        printCache(cache, 0, 0, 0, indiceSubstituido);
    }

    exibirMetricas();
}

// 2. Simulação da Política LRU
void simularLRU(int dados[], int tamanhoDados) {
    BlocoCache cache[TAMANHO_CACHE];
    for (int i = 0; i < TAMANHO_CACHE; i++) {
        cache[i].dado = -1;
        cache[i].ultima_utilizacao = 0;
    }

    total_hits = total_misses = 0;
    int tempo = 0;

    printf("\nPolítica LRU:\n");
    for (int i = 0; i < tamanhoDados; i++) {
        tempo++;
        int encontrado = 0, indiceLRU = 0, menorTempo = tempo;
        int indiceSubstituido = -1;

        for (int j = 0; j < TAMANHO_CACHE; j++) {
            if (cache[j].dado == dados[i]) {
                encontrado = 1;
                cache[j].ultima_utilizacao = tempo;
                total_hits++;
                break;
            }
            if (cache[j].dado == -1 || cache[j].ultima_utilizacao < menorTempo) {
                menorTempo = cache[j].ultima_utilizacao;
                indiceLRU = j;
            }
        }

        if (!encontrado) {
            total_misses++;
            indiceSubstituido = indiceLRU;
            cache[indiceLRU].dado = dados[i];
            cache[indiceLRU].ultima_utilizacao = tempo;
        }

        printf("Dado: %d\n", dados[i]);
        printCache(cache, tempo, 1, 0, indiceSubstituido);
    }

    exibirMetricas();
}

// 3. Simulação da Política LFU
void simularLFU(int dados[], int tamanhoDados) {
    BlocoCache cache[TAMANHO_CACHE];
    for (int i = 0; i < TAMANHO_CACHE; i++) {
        cache[i].dado = -1;
        cache[i].frequencia = 0;
    }

    total_hits = total_misses = 0;
    printf("\nPolítica LFU:\n");

    for (int i = 0; i < tamanhoDados; i++) {
        int encontrado = 0, indiceLFU = 0, menorFrequencia = cache[0].frequencia;
        int indiceSubstituido = -1;

        for (int j = 0; j < TAMANHO_CACHE; j++) {
            if (cache[j].dado == dados[i]) {
                encontrado = 1;
                cache[j].frequencia++;
                total_hits++;
                break;
            }
        }

        if (!encontrado) {
            total_misses++;
            for (int j = 0; j < TAMANHO_CACHE; j++) {
                if (cache[j].dado == -1 || cache[j].frequencia < menorFrequencia) {
                    menorFrequencia = cache[j].frequencia;
                    indiceLFU = j;
                }
            }

            indiceSubstituido = indiceLFU;
            cache[indiceLFU].dado = dados[i];
            cache[indiceLFU].frequencia = 1;
        }

        printf("Dado: %d\n", dados[i]);
        printCache(cache, 0, 0, 1, indiceSubstituido);
    }

    exibirMetricas();
}
// 4. Simulação da Política RANDOM
void simularRANDOM(int dados[], int tamanhoDados) {
    BlocoCache cache[TAMANHO_CACHE];
    for (int i = 0; i < TAMANHO_CACHE; i++) {
        cache[i].dado = -1; // Inicializa a cache como vazia
    }

    srand(time(NULL)); // Inicializa o gerador de números aleatórios
    total_hits = total_misses = 0; // Reinicia as métricas

    printf("\nPolítica RANDOM:\n");
    for (int i = 0; i < tamanhoDados; i++) {
        int encontrado = 0, indiceSubstituido = -1;

        // Verifica se o dado já está na cache
        for (int j = 0; j < TAMANHO_CACHE; j++) {
            if (cache[j].dado == dados[i]) {
                encontrado = 1;
                total_hits++; // Contabiliza acerto
                break;
            }
        }

        // Se não encontrado, substitui aleatoriamente
        if (!encontrado) {
            total_misses++; // Contabiliza falha
            int indiceAleatorio = rand() % TAMANHO_CACHE; // Seleciona um índice aleatório
            indiceSubstituido = indiceAleatorio;         // Salva o índice substituído
            cache[indiceAleatorio].dado = dados[i];
        }

        printf("Dado: %d\n", dados[i]);
        printCache(cache, 0, 0, 0, indiceSubstituido);
    }

    exibirMetricas(); // Exibir métricas no final da simulação
}
void gerarDadosAleatorios(int dados[], int tamanho, int intervaloMin, int intervaloMax) {
    srand(time(NULL)); // Inicializa a semente para números aleatórios
    for (int i = 0; i < tamanho; i++) {
        dados[i] = (rand() % (intervaloMax - intervaloMin + 1)) + intervaloMin;
    }
}

int main() {
    int tamanhoDados;
    int intervaloMin = 1, intervaloMax = 80;

    printf("Digite a quantidade de dados a serem gerados: ");
    scanf("%d", &tamanhoDados);

    int dados[tamanhoDados]; // Array para armazenar os dados gerados

    // Gerar dados aleatórios
    gerarDadosAleatorios(dados, tamanhoDados, intervaloMin, intervaloMax);

    printf("\nDados gerados:\n");
    for (int i = 0; i < tamanhoDados; i++) {
        printf("%d ", dados[i]);
    }
    printf("\n");

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
