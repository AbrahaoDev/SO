#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define NUM_THREADS 5

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
void* soma(void* args) {
    argum* ms = (argum*)args;
    int m = ms->m;
    int n = ms->n;
    int i, j;
    ms->d = (int*)malloc(m * n * sizeof(int));

    // Somando a matriz A com a B
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            ms->d[i * n + j] = ms->a[i * n + j] + ms->b[i * n + j];
        }
    }
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
void* produto(void* args) {
    argum* ms = (argum*)args;
    int m = ms->m;
    int n = ms->n;
    int i, j, z;
    ms->e = (int*)malloc(m * n * sizeof(int));

    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            ms->e[i * n + j] = 0;
            for (z = 0; z < n; z++) {
                ms->e[i * n + j] += ms->c[i * n + z] * ms->d[i * n + z];
            }
        }
    }
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
void* reduc(void* args) {
    argum* ms = (argum*)args;
    int m = ms->m;
    int n = ms->n;
    int i, j, result = 0;

    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            result += ms->e[i * n + j];
        }
    }
    printf("%d\n", result);
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
