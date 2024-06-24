# Código para matéria de SO.
O nosso código consiste em um programa que lerá 3 matrizes (A,B,C). Após a leitura das matrizes, o programa somará a matriz A com a matriz B e gravará o resultado em uma matriz D. Após isso, será feito o produto entre a matriz C e a matriz D, resultando na matriz E que também será gravada em um arquivo. Todas as matrizes possuem a extensão .dat e as matrizes A,B e C terão de estar previamente na mesma pasta do programa.

# Cabeçalhos
```
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

```

A primeira coisa a se fazer é adicionar os cabeçalho necessários para a construção do programa. Vamos usar a biblioteca padrão, a stdio.h, para funções de entrada/saida; A biblioteca stdlib.h para alocação dinâmica; math.h para funções matemáticas e a pthread.h para a progamação paralela com o uso de threads. 

# Definindo os parametros e quantidade de threads
```
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
```
Em seguida, vamos definir o número de threads que serão utilizadas no código. Apesar do programa ter 8 funções que possam ser utilizadas como threads, nós só vamos definir 5 threads, já que algumas funções dependem do resultado da outras e não poderão executar simultaneamente.
Vamos também definir uma estrutura com todos os parametros utilizados na função. Usaremos uma estrutura para passar os parametros para a função porque uma função thread pode receber apenas um ponteiro do tipo vazio.

# Função para achar o tamanho da matriz
```
int num() {
    int num, qtd = 0;
    FILE* arq2 = fopen("arqA.dat", "r");
    while (fscanf(arq2, "%d", &num) == 1) {
        qtd++;
    }
    fclose(arq2);
    return qtd;
}
```
A primeira função do nosso código é que irá calcular o tamanho das matrizes a serem utilizadas. Primeira ela declara duas váriaveis do tipo int, num e qtd(com o valor 0 atribuido). Abrimos o arquivo da matriz A (arqA.dat) em modo de leitura utilizando o fopen. Em seguida criamos um laço do tipo While que irá rodar todo o arquivo. Toda vez que ele ler um numero inteiro (int), irá somar 1 à variavel qtd. Em resumo, o laço irá contar quantos numeros a matriz possue. Sabemos pelo enunciado do projeto que serão testadas matrizes NxN, ou seja, matrizes com o mesmo tamanho de altura e largura, logo podemos afirmar que altura/largura da matriz é a raiz quadrada da quantidade de numeros que possue a matriz que recebemos.
