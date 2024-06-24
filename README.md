# Código para matéria de SO.
O nosso código consiste em um programa que lerá 3 matrizes (A,B,C). Após a leitura das matrizes, o programa somará a matriz A com a matriz B e gravará o resultado em uma matriz D. Após isso, será feito o produto entre a matriz C e a matriz D, resultando na matriz E que também será gravada em um arquivo. Todas as matrizes possuem a extensão .dat e as matrizes A,B e C terão de estar previamente na mesma pasta do programa.
``
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
``
