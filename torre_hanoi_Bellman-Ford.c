#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>


#define NUM_CONFIGS 81 // 3^4 possíveis configurações para 4 discos e 3 pinos
#define INF INT_MAX

// Função para converter a configuração em um índice único
int config_to_index(int config[4]) {
    return config[0] * 27 + config[1] * 9 + config[2] * 3 + config[3];
}


// Função para converter um índice de volta para configuração
void index_to_config(int index, int config[4]) {
    config[0] = index / 27;
    config[1] = (index / 9) % 3;
    config[2] = (index / 3) % 3;
    config[3] = index % 3;
}


// Função para verificar se uma configuração pode ser alcançada por um movimento legal
int movimento_valido(int config1[4], int config2[4]) {
    int i, diferenca = 0;
    for (i = 0; i < 4; i++) {
        if (config1[i] != config2[i]) {
            diferenca++;
        }
    }
    return diferenca == 1;
}


// Função para aplicar o Algoritmo de Bellman-Ford
void bellman_ford(int grafo[NUM_CONFIGS][NUM_CONFIGS], int inicio, int fim) {
    int distancia[NUM_CONFIGS], antecessor[NUM_CONFIGS], i , u, v;

    for (i = 0; i < NUM_CONFIGS; i++) {
        distancia[i] = INF;
        antecessor[i] = -1;
    }
    distancia[inicio] = 0;

    for (i = 0; i < NUM_CONFIGS - 1; i++) {
        for (u = 0; u < NUM_CONFIGS; u++) {
            for (v = 0; v < NUM_CONFIGS; v++) {
                if (grafo[u][v] && distancia[u] != INF && distancia[u] + grafo[u][v] < distancia[v]) {
                    distancia[v] = distancia[u] + grafo[u][v];
                    antecessor[v] = u;
                }
            }
        }
    }

    printf("Menor caminho de %d para %d: %d movimentos\n", inicio, fim, distancia[fim]);

    // Exibir o caminho percorrido
    int caminho[NUM_CONFIGS], count = 0;
    for (v = fim; v != -1; v = antecessor[v]) {
        caminho[count++] = v;
    }
    printf("Caminho percorrido: ");
    for (i = count - 1; i >= 0; i--) {
        printf("%d", caminho[i]);
        if (i > 0) {
            printf(" -> ");
        }
    }
    printf("\n");
}

int main() {
    int grafo[NUM_CONFIGS][NUM_CONFIGS] = {{0}};
    int config1[4], config2[4], i, j;

    // Gerar o grafo com as conexões válidas
    for (i = 0; i < NUM_CONFIGS; i++) {
        index_to_config(i, config1);
        for (j = 0; j < NUM_CONFIGS; j++) {
            index_to_config(j, config2);
            if (movimento_valido(config1, config2)) {
                grafo[i][j] = 1;
            }
        }
    }

    int inicio = config_to_index((int[]){0, 0, 0, 0}); // Todos os discos no pino A
    int fim = config_to_index((int[]){2, 2, 2, 2});    // Todos os discos no pino C

    // Medir o tempo de execução usando gettimeofday
    struct timeval start, end;
    gettimeofday(&start, NULL);
    bellman_ford(grafo, inicio, fim);
    gettimeofday(&end, NULL);

    long segundos = end.tv_sec - start.tv_sec;
    long microssegundos = end.tv_usec - start.tv_usec;

    if (microssegundos < 0) {
        segundos -= 1;
        microssegundos += 1000000;
    }

    printf("Tempo gasto: %ld segundos e %ld microssegundos\n", segundos, microssegundos);

    return 0;
}
