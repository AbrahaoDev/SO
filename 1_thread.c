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
void reada(argum* ms) {
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
}

// Função para ler a matriz B
void readb(argum* ms) {
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
}

// Função para ler a matriz C
void readc(argum* ms) {
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
}

// Função para somar as matrizes A e B
void soma(argum* ms) {
   

    int m = ms->m;
    int n = ms->n;
    ms->d = (int*)malloc(m * n * sizeof(int));

  

    // Somando a matriz A com a B
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            ms->d[i * n + j] = ms->a[i * n + j] + ms->b[i * n + j];
        }
    }

 
}

// Função para gravar a matriz D
void writed(argum* ms) {
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
}

// Função para calcular o produto das matrizes C e D (thread)
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
void writee(argum* ms) {
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
}

// Função para calcular a redução da matriz E
void reduc(argum* ms) {
    

    int m = ms->m;
    int n = ms->n;
    int result = 0;

    

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            result += ms->e[i * n + j];
        }
    }

   
    printf("%d\n", result);
}

int main() {
    clock_t inicio, fim;
    double tempoTotal;

    int m = sqrt(num());
    int n = sqrt(num());
    argum ms;
    ms.m = m;
    ms.n = n;

    inicio = clock();

   
    reada(&ms);
    readb(&ms);
    readc(&ms);

    soma(&ms);

    // Criar thread para função produto
    pthread_t thread_produto;
    pthread_create(&thread_produto, NULL, produto, (void*)&ms);
    pthread_join(thread_produto, NULL);

    writed(&ms);
   
    writee(&ms);

    reduc(&ms);

    // Medir tempo total
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
