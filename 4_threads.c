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
void* reada(void* arg) {
    argum* ms = (argum*)arg;
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
void* readb(void* arg) {
    argum* ms = (argum*)arg;
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
void* readc(void* arg) {
    argum* ms = (argum*)arg;
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
void* writed(void* arg) {
    argum* ms = (argum*)arg;
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
void* writee(void* arg) {
    clock_t inicio, fim;
    double tempoTotal;
    argum* ms = (argum*)arg;
    int m = ms->m;
    int n = ms->n;
    int i, j;
    FILE* arqE = fopen("arqE.dat", "w");
    inicio = clock();

    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            fprintf(arqE, "%d ", ms->e[i * n + j]);
        }
        fprintf(arqE, "\n");
    }
    fim = clock() - inicio;
    tempoTotal = ((double) fim) / CLOCKS_PER_SEC;
    printf("A função writee() gastou %f segundos. \n", tempoTotal);

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

    pthread_t threads[5];

  
    inicio = clock();

    // Criando threads
    pthread_create(&threads[0], NULL, reada, (void*)&ms);
    pthread_create(&threads[1], NULL, readb, (void*)&ms);
    pthread_create(&threads[2], NULL, readc, (void*)&ms);

    // Esperarndo as threads
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    // Executar as threads restantes 
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

    // Medir tempo total
    fim = clock() - inicio;
    tempoTotal = ((double) fim) / CLOCKS_PER_SEC;
    printf("O programa gastou %f segundos no total. \n", tempoTotal);

    //Liberando memória
    free(ms.a);
    free(ms.b);
    free(ms.c);
    free(ms.d);
    free(ms.e);

    return 0;
}
