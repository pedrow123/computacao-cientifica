#ifndef MATRIZ_H
#define MATRIZ_H
#include "intervaloOp.h"

typedef struct matriz {
    intervalo_t **A;
    intervalo_t *B;
    intervalo_t *X;
    int tam;
} matriz_t;

struct pontos {
    double x;
    double y;
};
typedef struct pontos pontos_t;

typedef struct indice {
    int i;
    int j;
} indice_t;

matriz_t *criaMatriz(int n);
void recebeEntrada(matriz_t *matriz);
void printaMatriz(matriz_t *matriz);
void retrossubs(matriz_t *matriz);
void eliminacaoGauss(matriz_t *matriz);
void eliminacaoGaussSemMultiplicador(matriz_t *matriz);
void eliminacaoGaussSemPivoteamento(matriz_t *matriz);
intervalo_t* calculaResiduo(matriz_t *matriz, pontos_t *xy, int k);
void libera(matriz_t *matriz, intervalo_t* residuo);
void copiaMatriz(matriz_t *m1, matriz_t *m2);
void criaSL(pontos_t *xy, matriz_t *SL, int k, int n);

#endif
