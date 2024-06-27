/*O nosso código consiste em um programa que lerá 3 matrizes (A,B,C). Após a leitura das matrizes,
o programa somará a matriz A com a matriz B e gravará o resultado em uma matriz D. Após isso,
será feito o produto entre a matriz C e a matriz D, resultando na matriz E que também será gravada em um arquivo.
Todas as matrizes possuem a extensão .dat e as matrizes A,B e C terão de estar previamente na mesma pasta do programa. */




//Bibliotecas usadas, stdlib.h para alocação dinâmicae,
//math.h para determinadas funções matemáticas e Pthreads para o programação paralela
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
//definindo o numero de threads à serem utilizadas
#define NUM_THREADS 5
//estrutura com todos os parâmetros que serão utilizadas nas threads
typedef struct {
    int m;
    int n;
    int *a;
    int *b;
    int *c;
    int *d;
    int *e;
} argum;

// Função para achar o tamanho da matriz
int num() {
    int num, qtd = 0;
    FILE* arq2 = fopen("arqA.dat", "r");
    while (fscanf(arq2, "%d", &num) == 1) {
        qtd++;
    }
    fclose(arq2);
    return qtd;
}

// Lendo a matriz A
void* reada(void* args) {
    argum* ms = (argum*)args;
    int m = ms->m;
    int n = ms->n;
    int i, j;
    ms->a = (int*)malloc(m * n * sizeof(int));
    FILE* arqA = fopen("arqA.dat", "r");

    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            fscanf(arqA, "%d", &ms->a[i * n + j]);
        }
    }
    fclose(arqA);
    pthread_exit(NULL);
}

// Lendo a matriz B
void* readb(void* args) {
    argum* ms = (argum*)args;
    int m = ms->m;
    int n = ms->n;
    int i, j;
    ms->b = (int*)malloc(m * n * sizeof(int));
    FILE* arqB = fopen("arqB.dat", "r");

    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            fscanf(arqB, "%d", &ms->b[i * n + j]);
        }
    }
    fclose(arqB);
    pthread_exit(NULL);
}

// Lendo a matriz C
void* readc(void* args) {
    argum* ms = (argum*)args;
    int m = ms->m;
    int n = ms->n;
    int i, j;
    ms->c = (int*)malloc(m * n * sizeof(int));
    FILE* arqC = fopen("arqC.dat", "r");

    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            fscanf(arqC, "%d", &ms->c[i * n + j]);
        }
    }
    fclose(arqC);
    pthread_exit(NULL);
}

// Função para somar a matriz A e a B
void* soma(void* arg) {
    clock_t inicio, fim;
    double tempoTotal;

    argum* ms = (argum*)arg;
    int m = ms->m;
    int n = ms->n;
    int i, j;
    ms->d = (int*)malloc(m * n * sizeof(int));

    inicio = clock();

    // Somando a matriz A com a B
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            ms->d[i * n + j] = ms->a[i * n + j] + ms->b[i * n + j];
        }
    }

    fim = clock() - inicio;
    tempoTotal = ((double) fim) / CLOCKS_PER_SEC;
    printf("A função soma() gastou %f segundos. \n", tempoTotal);

    pthread_exit(NULL);
}

// Função para gravar matriz D
void* writed(void* args) {
    argum* ms = (argum*)args;
    int m = ms->m;
    int n = ms->n;
    int i, j;
    FILE* arqD = fopen("arqD.dat", "w");

    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            fprintf(arqD, "%d ", ms->d[i * n + j]);
        }
        fprintf(arqD, "\n");
    }
    fclose(arqD);
    pthread_exit(NULL);
}

// Função para achar o produto entre a matriz C e D
void* produto(void* arg) {
    clock_t inicio, fim;
    double tempoTotal;

    argum* ms = (argum*)arg;
    int m = ms->m;
    int n = ms->n;
    int i, j, z;
    ms->e = (int*)malloc(m * n * sizeof(int));

    inicio = clock();

    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            ms->e[i * n + j] = 0;
            for (z = 0; z < n; z++) {
                ms->e[i * n + j] += ms->c[i * n + z] * ms->d[i * n + z];
            }
        }
    }

    fim = clock() - inicio;
    tempoTotal = ((double) fim) / CLOCKS_PER_SEC;
    printf("A função produto() gastou %f segundos. \n", tempoTotal);

    pthread_exit(NULL);
}

// Função para salvar matriz E
void* writee(void* args) {
    argum* ms = (argum*)args;
    int m = ms->m;
    int n = ms->n;
    int i, j;
    FILE* arqE = fopen("arqE.dat", "w");

    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            fprintf(arqE, "%d ", ms->e[i * n + j]);
        }
        fprintf(arqE, "\n");
    }
    fclose(arqE);
    pthread_exit(NULL);
}

// Função para redução
void* reduc(void* arg) {
    clock_t inicio, fim;
    double tempoTotal;

    argum* ms = (argum*)arg;
    int m = ms->m;
    int n = ms->n;
    int i, j, result = 0;

    inicio = clock();

    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            result += ms->e[i * n + j];
        }
    }

    fim = clock() - inicio;
    tempoTotal = ((double) fim) / CLOCKS_PER_SEC;
    printf("A função reduc() gastou %f segundos. \n", tempoTotal);

    printf("Resultado da redução: %d\n", result);
    pthread_exit(NULL);
}

int main() {
    int m = sqrt(num());
    int n = sqrt(num());
    argum ms;
    ms.m = m;
    ms.n = n;

    pthread_t threads[NUM_THREADS];

    //Criando as threads
    pthread_create(&threads[0], NULL, reada, (void*)&ms);
    pthread_create(&threads[1], NULL, readb, (void*)&ms);
    pthread_create(&threads[2], NULL, readc, (void*)&ms);

    //Esperando
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    //Criando as threads restantes
    pthread_create(&threads[3], NULL, soma, (void*)&ms);
    pthread_join(threads[3], NULL);
    pthread_create(&threads[4], NULL, writed, (void*)&ms);
    pthread_join(threads[4], NULL);

    pthread_create(&threads[3], NULL, produto, (void*)&ms);
    pthread_join(threads[3], NULL);
    pthread_create(&threads[4], NULL, writee, (void*)&ms);
    pthread_join(threads[4], NULL);

    pthread_create(&threads[3], NULL, reduc, (void*)&ms);
    pthread_join(threads[3], NULL);

    //Liberando memória
    free(ms.a);
    free(ms.b);
    free(ms.c);
    free(ms.d);
    free(ms.e);

    return 0;
}
