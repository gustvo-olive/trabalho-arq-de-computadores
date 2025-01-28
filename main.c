#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_CACHE 4
/*
Disciplina: Arquitetura de Computadores

Grupo 3: Simulação de Políticas de substituição de Cache

Alunos:
Beatriz Pinheiro de Azevedo
Gustavo de Oliveira Rego Morais
José João Monteiro Costa
Wesley Barbosa do Nascimento
*/

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

// Politicas de Substituicao:

// 1. Simulação da Politica FIFO
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

        int indiceSubstituido = -1; // Inicialmente, nenhum dado foi substituído
        if (!encontrado) {
            indiceSubstituido = indice; // Salva o índice que será substituído
            cache[indice].dado = dados[i];
            indice = (indice + 1) % TAMANHO_CACHE; // Atualiza o índice para FIFO
        }
        printf("Dado: %d\n",dados[i] );
        printCache(cache, 0, 0, 0, indiceSubstituido); // Passa 0 para não mostrar "Não usado há" ou "Frequencia", e passa o indice que foi
    }
}

// 2.Simulação da Politica LRU
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

        int indiceSubstituido = -1; // Inicialmente, nenhum dado foi substituído

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
            indiceSubstituido = indiceLRU; // Salva o índice do dado que será substituído
            cache[indiceLRU].dado = dados[i];
            cache[indiceLRU].ultima_utilizacao = tempo;
        }
        printf("Dado: %d\n",dados[i] );
        printCache(cache, tempo, 1, 0, indiceSubstituido); // Mostra "Não usado há"
        
    }
}

// 3. Simulação da Politica RANDOM
void simularRANDOM(int dados[], int tamanhoDados) {
    BlocoCache cache[TAMANHO_CACHE];
    for (int i = 0; i < TAMANHO_CACHE; i++) {
        cache[i].dado = -1; // Inicializa a cache como vazia
    }

    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    printf("\nPolitica RANDOM:\n");
    for (int i = 0; i < tamanhoDados; i++) {
        int encontrado = 0;
        int indiceSubstituido = -1;
        for (int j = 0; j < TAMANHO_CACHE; j++) {
            if (cache[j].dado == dados[i]) {
                encontrado = 1;
                break;
            }
        }
        if (!encontrado) {
            int indiceAleatorio = rand() % TAMANHO_CACHE; // Seleciona um índice aleatório
            indiceSubstituido = indiceAleatorio;         // Salva o índice substituído
            cache[indiceAleatorio].dado = dados[i];
        }

        printCache(cache, 0, 0, 0, indiceSubstituido);
        printf("Dado: %d\n",dados[i] );
        printCache(cache, 0, 0, 0, indiceSubstituido);// Passa 0 para não mostrar "Não usado há" ou "Frequencia"
    }
}

// 4. Simulação da Politica LFU
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
        int indiceSubstituido = -1; // Inicialmente, nenhum dado foi substituído
        
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
            for (int j = 0; j < TAMANHO_CACHE; j++) {
                if (cache[j].dado == -1 || cache[j].frequencia < menorFrequencia) {
                    menorFrequencia = cache[j].frequencia;
                    indiceLFU = j;
                }
            }

            indiceSubstituido = indiceLFU; // Salva o índice do dado substituído
            cache[indiceLFU].dado = dados[i];
            cache[indiceLFU].frequencia = 1; // Reseta a frequência
        }
        printf("Dado: %d\n",dados[i] );
        printCache(cache, 0, 0, 1, indiceSubstituido); // Mostra "Frequencia""
    }
}

void gerarDadosAleatorios(int dados[], int tamanho, int intervaloMin, int intervaloMax) {
    srand(time(NULL)); // Inicializa a semente para números aleatórios
    for (int i = 0; i < tamanho; i++) {
        dados[i] = (rand() % (intervaloMax - intervaloMin + 1)) + intervaloMin;
    }
}

int main() {
    int tamanhoDados;
    int intervaloMin = 1, intervaloMax = 7;

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

