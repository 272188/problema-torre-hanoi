/*
Um problema muito conhecido na ci�ncias da computa��o � o da Torre de Han�i. O problema cl�ssico da Torre de 
Han�i em, dados n discos e 3 pinos, mover com o menor n�mero poss�vel de movimentos todos os n discos de um 
suporte origem, de acordo com as seguinte condi��es: apenas um disco pode ser movimentado de cada vez; os discos 
movidos dever�o ser apoiados sempre em discos de maior tamanho ou na base de algum pino. A Figura 1 exibe os 
movimentos do desafio da Torre de Han�i. Por conven��o uma configura��o de discos ser� representada por um vetor 
com tantas posi��es quantas forem os discos. Na posi��o do disco ser� marcado o pino onde o disco est� assentado, 
como mostrado na Figuras 2, 3 e 4. A Figura 5 exemplifica como formar um grafo que representa as possibilidade de 
movimentos dos discos. Cada v�rtice representa uma diferente configura��o do desafio. As configura��es s�o ligadas 
por arestas se uma configura��o pode ser alcan�ada a partir de outra pelo movimento legal de um disco. De acordo 
com o exposto, modele o grafo que represente o grafo de movimentos do desafio da Torre de Han�i para o caso de 4 
discos. Em seguida crie o grafo usando matriz de adjac�ncia. Depois dada uma determinada configura��o dos discos 
encontre o menor caminho para o resultado final usando o Algoritmo de Dijskstra, para isso coloque valor 1 em cada 
uma das arestas. Contabilize o tempo gasto para encontrar a solu��o. 
*/


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>

#define NUM_CONFIGS 81 // 3^4 configura��es para 4 discos e 3 pinos
#define INF INT_MAX

// Fun��o para converter a configura��o em um �ndice �nico
int config_to_index(int config[4]) {
    return config[0] * 27 + config[1] * 9 + config[2] * 3 + config[3];
}


// Fun��o para converter um �ndice de volta para configura��o
void index_to_config(int index, int config[4]) {
    config[0] = index / 27;
    config[1] = (index / 9) % 3;
    config[2] = (index / 3) % 3;
    config[3] = index % 3;
}


// Fun��o para verificar se uma configura��o pode ser alcan�ada por um movimento legal
int movimento_valido(int config1[4], int config2[4]) {
    int diferenca = 0;
    int i;
    for (i = 0; i < 4; i++) {
        if (config1[i] != config2[i]) {
            diferenca++;
        }
    }
    return diferenca == 1;
}

// Fun��o para aplicar o Algoritmo de Dijkstra
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
    int config1[4], config2[4], i, j;
    struct timeval start, end;

    // Gerar o grafo com as conex�es v�lidas
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

