/*
Disciplina: Arquitetura de Computadores

Grupo 3: Simulação de Políticas de substituição de Cache

Alunos:
Beatriz Pinheiro de Azevedo
Gustavo de Oliveira Rego Morais
José João Monteiro Costa
Wesley Barbosa do Nascimento
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAMANHO_CACHE 4


// Variáveis globais para as métricas
int total_hits = 0;
int total_misses = 0;

typedef struct {
    int dado;
    int ultima_utilizacao; // Para LRU
    int frequencia;        // Para LFU
} BlocoCache;

// Função para dar print no estado da cache
void printCache(BlocoCache cache[], int tempoAtual, int mostrarUltimaUtilizacao, int mostrarFrequencia, int indiceSubstituido) {
    for (int i = 0; i < TAMANHO_CACHE; i++) {
        if (cache[i].dado != -1) {
            if (i == indiceSubstituido) {
                // Destacar em vermelho o dado substituído
                printf("\033[31m"); // Cor vermelha
                printf("Cache[%d]: %d ", i, cache[i].dado);
                printf("\033[0m"); // Resetar a cor
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
    int verbose = (tamanhoDados <= 99); // Se true, imprime estado intermediário

    printf("\nPolitica FIFO:\n");
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

        // Se o modo verbose estiver ativo, imprime o estado da cache
        if (verbose) {
            printf("Dado: %d\n", dados[i]);
            printCache(cache, 0, 0, 0, indiceSubstituido);
        }
    }

    // Se não estiver em modo verbose, podemos imprimir uma mensagem final da cache, se desejado
    if (!verbose) {
        printf("Estado final da cache:\n");
        printCache(cache, 0, 0, 0, -1);
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
    int verbose = (tamanhoDados <= 99);

    printf("\nPolitica LRU:\n");
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

        if (verbose) {
            printf("Dado: %d\n", dados[i]);
            printCache(cache, tempo, 1, 0, indiceSubstituido);
        }
    }

    if (!verbose) {
        printf("Estado final da cache:\n");
        printCache(cache, tempo, 1, 0, -1);
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
    int verbose = (tamanhoDados <= 99);

    printf("\nPolitica LFU:\n");
    for (int i = 0; i < tamanhoDados; i++) {
        int encontrado = 0, indiceLFU = 0;
        int menorFrequencia = cache[0].frequencia;
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

        if (verbose) {
            printf("Dado: %d\n", dados[i]);
            printCache(cache, 0, 0, 1, indiceSubstituido);
        }
    }

    if (!verbose) {
        printf("Estado final da cache:\n");
        printCache(cache, 0, 0, 1, -1);
    }

    exibirMetricas();
}

// 4. Simulação da Política RANDOM
void simularRANDOM(int dados[], int tamanhoDados) {
    BlocoCache cache[TAMANHO_CACHE];
    for (int i = 0; i < TAMANHO_CACHE; i++) {
        cache[i].dado = -1;
    }

    srand(time(NULL));
    total_hits = total_misses = 0;
    int verbose = (tamanhoDados <= 99);

    printf("\nPolitica RANDOM:\n");
    for (int i = 0; i < tamanhoDados; i++) {
        int encontrado = 0, indiceSubstituido = -1;

        for (int j = 0; j < TAMANHO_CACHE; j++) {
            if (cache[j].dado == dados[i]) {
                encontrado = 1;
                total_hits++;
                break;
            }
        }

        if (!encontrado) {
            total_misses++;
            int indiceAleatorio = rand() % TAMANHO_CACHE;
            indiceSubstituido = indiceAleatorio;
            cache[indiceAleatorio].dado = dados[i];
        }

        if (verbose) {
            printf("Dado: %d\n", dados[i]);
            printCache(cache, 0, 0, 0, indiceSubstituido);
        }
    }

    if (!verbose) {
        printf("Estado final da cache:\n");
        printCache(cache, 0, 0, 0, -1);
    }

    exibirMetricas();
}


// Função para gerar dados aleatórios
void gerarDadosAleatorios(int dados[], int tamanho, int intervaloMin, int intervaloMax) {
    for (int i = 0; i < tamanho; i++) {
        dados[i] = (rand() % (intervaloMax - intervaloMin + 1)) + intervaloMin;
    }
}

// Função auxiliar para ler um inteiro de forma segura
int getIntInput(const char *prompt) {
    int value;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            // Remove a quebra de linha, se houver
            buffer[strcspn(buffer, "\n")] = 0;
            // Tenta converter para inteiro
            if (sscanf(buffer, "%d", &value) == 1) {
                return value;
            } else {
                printf("Entrada invalida. Por favor, insira um numero inteiro.\n");
            }
        } else {
            printf("Erro na leitura da entrada.\n");
            exit(1);
        }
    }
}

int main() {
    int tamanhoDados, intervaloMin, intervaloMax, escolha;

    // Leitura segura da quantidade de dados
    tamanhoDados = getIntInput("Digite a quantidade de dados a serem gerados: ");
    
    // Leitura segura do intervalo mínimo e máximo dos números aleatórios
    intervaloMin = getIntInput("Digite o intervalo MINIMO dos numeros aleatorios: ");
    intervaloMax = getIntInput("Digite o intervalo MAXIMO dos numeros aleatorios: ");
    
    // Valida se o intervalo minimo é menor ou igual ao maximo
    while (intervaloMin > intervaloMax) {
        printf("Intervalo MINIMO deve ser menor ou igual ao MAXIMO. Por favor, insira novamente.\n");
        intervaloMin = getIntInput("Digite o intervalo MINIMO dos numeros aleatorios: ");
        intervaloMax = getIntInput("Digite o intervalo MAXIMO dos numeros aleatorios: ");
    }

    // Criação do array de dados com tamanho dinâmico
    int dados[tamanhoDados];

    // Inicializa a semente para números aleatórios
    srand(time(NULL));
    
    // Gerar dados aleatórios
    gerarDadosAleatorios(dados, tamanhoDados, intervaloMin, intervaloMax);

    printf("\nDados gerados:\n");
    for (int i = 0; i < tamanhoDados; i++) {
        printf("%d ", dados[i]);
    }
    printf("\n");

    // Leitura segura para escolha da política
    escolha = getIntInput("\nEscolha a politica de substituicao:\n1 - FIFO\n2 - LRU\n3 - LFU\n4 - RANDOM\nDigite sua escolha: ");

    // Chamada da política escolhida
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
            printf("Escolha invalida!\n");
    }

    return 0;
}
