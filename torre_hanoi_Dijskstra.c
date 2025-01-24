#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>

#define NUM_CONFIGS 27 // 3^3 possíveis configurações para 3 discos e 3 pinos
#define INF INT_MAX

// Função para converter a configuração em um índice único
int config_to_index(int config[3]) {
    return config[0] * 9 + config[1] * 3 + config[2];
}

// Função para converter um índice de volta para configuração
void index_to_config(int index, int config[3]) {
    config[0] = index / 9;
    config[1] = (index / 3) % 3;
    config[2] = index % 3;
}

// Função para verificar se uma configuração pode ser alcançada por um movimento legal
int movimento_valido(int config1[3], int config2[3]) {
    int diferenca = 0;
    int i;
    for (i = 0; i < 3; i++) {
        if (config1[i] != config2[i]) {
            diferenca++;
        }
    }
    return diferenca == 1;
}

// Função para aplicar o Algoritmo de Dijkstra
void dijkstra(int grafo[NUM_CONFIGS][NUM_CONFIGS], int inicio, int fim) {
    int distancia[NUM_CONFIGS], visitado[NUM_CONFIGS], antecessor[NUM_CONFIGS], i, v;
    for (i = 0; i < NUM_CONFIGS; i++) {
        distancia[i] = INF;
        visitado[i] = 0;
        antecessor[i] = -1;
    }
    distancia[inicio] = 0;

    for (i = 0; i < NUM_CONFIGS - 1; i++) {
        int min_distancia = INF, min_index;
        for (v = 0; v < NUM_CONFIGS; v++) {
            if (!visitado[v] && distancia[v] < min_distancia) {
                min_distancia = distancia[v];
                min_index = v;
            }
        }

        visitado[min_index] = 1;

        for (v = 0; v < NUM_CONFIGS; v++) {
            if (!visitado[v] && grafo[min_index][v] && distancia[min_index] != INF && distancia[min_index] + grafo[min_index][v] < distancia[v]) {
                distancia[v] = distancia[min_index] + grafo[min_index][v];
                antecessor[v] = min_index;
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
    int config1[3], config2[3], i, j;

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

    int inicio = config_to_index((int[]){0, 0, 0}); // Todos os discos no pino A
    int fim = config_to_index((int[]){2, 2, 2});    // Todos os discos no pino C

    // Medir o tempo de execução usando gettimeofday
    struct timeval start, end;
    gettimeofday(&start, NULL);
    dijkstra(grafo, inicio, fim);
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
