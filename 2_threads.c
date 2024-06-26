#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

typedef struct {
    int m;
    int n;
    int *a;
    int *b;
    int *c;
    int *d;
    int *e;
} argum;

// Função para ler o número de elementos nas matrizes dos arquivos
int num() {
    int num, qtd = 0;
    FILE* arqA = fopen("arqA.dat", "r");
    if (arqA == NULL) {
        perror("Erro ao abrir o arquivo arqA.dat");
        exit(EXIT_FAILURE);
    }
    while (fscanf(arqA, "%d", &num) == 1) {
        qtd++;
    }
    fclose(arqA);
    return qtd;
}

// Função para ler a matriz A
void* reada(void* arg) {
    argum* ms = (argum*)arg;
    int m = ms->m;
    int n = ms->n;
    ms->a = (int*)malloc(m * n * sizeof(int));
    FILE* arqA = fopen("arqA.dat", "r");
    if (arqA == NULL) {
        perror("Erro ao abrir o arquivo arqA.dat");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            fscanf(arqA, "%d", &ms->a[i * n + j]);
        }
    }
    fclose(arqA);
    pthread_exit(NULL);
}

// Função para ler a matriz B
void* readb(void* arg) {
    argum* ms = (argum*)arg;
    int m = ms->m;
    int n = ms->n;
    ms->b = (int*)malloc(m * n * sizeof(int));
    FILE* arqB = fopen("arqB.dat", "r");
    if (arqB == NULL) {
        perror("Erro ao abrir o arquivo arqB.dat");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            fscanf(arqB, "%d", &ms->b[i * n + j]);
        }
    }
    fclose(arqB);
    pthread_exit(NULL);
}

// Função para ler a matriz C
void* readc(void* arg) {
    argum* ms = (argum*)arg;
    int m = ms->m;
    int n = ms->n;
    ms->c = (int*)malloc(m * n * sizeof(int));
    FILE* arqC = fopen("arqC.dat", "r");
    if (arqC == NULL) {
        perror("Erro ao abrir o arquivo arqC.dat");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            fscanf(arqC, "%d", &ms->c[i * n + j]);
        }
    }
    fclose(arqC);
    pthread_exit(NULL);
}

// Função para somar as matrizes A e B
void* soma(void* arg) {
    clock_t inicio, fim;
    double tempoTotal;

    argum* ms = (argum*)arg;
    int m = ms->m;
    int n = ms->n;
    ms->d = (int*)malloc(m * n * sizeof(int));

    inicio = clock();

    // Somando a matriz A com a B
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            ms->d[i * n + j] = ms->a[i * n + j] + ms->b[i * n + j];
        }
    }

    fim = clock();
    tempoTotal = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("A função soma() gastou %f segundos. \n", tempoTotal);

    pthread_exit(NULL);
}

// Função para gravar a matriz D
void* writed(void* arg) {
    argum* ms = (argum*)arg;
    int m = ms->m;
    int n = ms->n;
    FILE* arqD = fopen("arqD.dat", "w");
    if (arqD == NULL) {
        perror("Erro ao abrir o arquivo arqD.dat");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            fprintf(arqD, "%d ", ms->d[i * n + j]);
        }
        fprintf(arqD, "\n");
    }
    fclose(arqD);
    pthread_exit(NULL);
}

// Função para calcular o produto das matrizes C e D
void* produto(void* arg) {
    clock_t inicio, fim;
    double tempoTotal;

    argum* ms = (argum*)arg;
    int m = ms->m;
    int n = ms->n;
    ms->e = (int*)malloc(m * n * sizeof(int));

    inicio = clock();

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            ms->e[i * n + j] = 0;
            for (int z = 0; z < n; z++) {
                ms->e[i * n + j] += ms->c[i * n + z] * ms->d[z * n + j];
            }
        }
    }

    fim = clock();
    tempoTotal = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("A função produto() gastou %f segundos. \n", tempoTotal);

    pthread_exit(NULL);
}

// Função para gravar a matriz E
void* writee(void* arg) {
    
    argum* ms = (argum*)arg;
    int m = ms->m;
    int n = ms->n;
    FILE* arqE = fopen("arqE.dat", "w");
    if (arqE == NULL) {
        perror("Erro ao abrir o arquivo arqE.dat");
        exit(EXIT_FAILURE);
    }

   

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            fprintf(arqE, "%d ", ms->e[i * n + j]);
        }
        fprintf(arqE, "\n");
    }
    fclose(arqE);

  
    pthread_exit(NULL);
}

// Função para calcular a redução da matriz E
void* reduc(void* arg) {
    clock_t inicio, fim;
    double tempoTotal;

    argum* ms = (argum*)arg;
    int m = ms->m;
    int n = ms->n;
    int result = 0;

    inicio = clock();

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            result += ms->e[i * n + j];
        }
    }

    fim = clock();
    tempoTotal = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("A função reduc() gastou %f segundos. \n", tempoTotal);

    printf("%d\n", result);
    pthread_exit(NULL);
}

int main() {
    clock_t inicio, fim;
    double tempoTotal;

    int m = sqrt(num());
    int n = sqrt(num());
    argum ms;
    ms.m = m;
    ms.n = n;

    pthread_t threads[4];

    inicio = clock();

    // Criar threads para ler matrizes A, B e C
    pthread_create(&threads[0], NULL, reada, (void*)&ms);
    pthread_create(&threads[1], NULL, readb, (void*)&ms);
    pthread_create(&threads[2], NULL, readc, (void*)&ms);

    // Esperar threads de leitura
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    // Criar threads para soma, produto, escrita de D e escrita de E
    pthread_create(&threads[0], NULL, soma, (void*)&ms);
    pthread_create(&threads[1], NULL, produto, (void*)&ms);
    pthread_create(&threads[2], NULL, writed, (void*)&ms);
    pthread_create(&threads[3], NULL, writee, (void*)&ms);

    // Esperar threads de operação
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    // Calcular tempo total
    fim = clock();
    tempoTotal = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("O programa gastou %f segundos no total. \n", tempoTotal);

    // Liberar memória alocada
    free(ms.a);
    free(ms.b);
    free(ms.c);
    free(ms.d);
    free(ms.e);

    return 0;
}
