#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <sys/time.h>
#include "matriz.h"
#include "likwid.h"
#define N 3

double timestamp(void){
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return((double)(tp.tv_sec+tp.tv_usec/1000000.0))*1000;
}

int main() {
    LIKWID_MARKER_INIT;
    double start, stop = 0.0;
    int tam = 0;
    int returnScanf = 0;
    returnScanf = scanf("%d", &tam);

    matriz_t *matriz = criaMatriz(tam);
    matriz_t *matriz2 = criaMatriz(tam);
    matriz_t *matriz3 = criaMatriz(tam);
    recebeEntrada(matriz);

    copiaMatriz(matriz, matriz2);
    copiaMatriz(matriz, matriz3);

    /*
    3
    3 2 4 1
    1 1 2 2
    4 3 -2 3

    4
    4 -1 0 -1 3
    -1 4 -1 0 6
    0 -1 4 -1 1
    -1 0 -1 4 12

    4
    -1 4 -1 0 6
    2 -1 4 -1 1 
    4 -1 0 -1 3 
    -1 0 -1 4 12
    */

    printf("ELIMINAÇÃO GAUSSIANA CLÁSSICA COM PIVOTEAMENTO\n");
    start = timestamp();
    LIKWID_MARKER_START("classica");
    eliminacaoGauss(matriz);
    retrossubs(matriz);
    calculaResiduo(matriz);
    LIKWID_MARKER_STOP("classica");
    stop = timestamp();

    printf("X = [");
    for(int i=0; i<matriz->tam; i++) {
        printf(" %f", matriz->X[i]);
    }
    printf(" ]\n");

    printf("RESIDUO = [");
    for (int i = 0; i<matriz->tam; i++) {
        printf(" %f", matriz->residuo[i]);
    }
    printf(" ]\n");
    printf("TEMPO = %f\n\n", stop - start);

    printf("ELIMINAÇÃO GAUSSIANA SEM MULTIPLICADOR\n");
    start, stop = 0;
    start = timestamp();
    LIKWID_MARKER_START("sem-multiplicador");
    eliminacaoGaussSemMultiplicador(matriz2);
    retrossubs(matriz2);
    calculaResiduo(matriz2);
    LIKWID_MARKER_STOP("sem-multiplicador");
    stop = timestamp();

    printf("X = [");
    for(int i=0; i<matriz2->tam; i++) {
        printf(" %f", matriz2->X[i]);
    }
    printf(" ]\n");

    printf("RESIDUO = [");
    for (int i = 0; i<matriz2->tam; i++) {
        printf(" %f", matriz2->residuo[i]);
    }
    printf(" ]\n");
    printf("TEMPO = %f\n\n", stop - start);

    printf("ELIMINAÇÃO GAUSSIANA SEM PIVOTEAMENTO\n");
    start, stop = 0;
    start = timestamp();
    LIKWID_MARKER_START("sem-pivo");
    eliminacaoGaussSemPivoteamento(matriz3);
    retrossubs(matriz3);
    calculaResiduo(matriz3);
    LIKWID_MARKER_STOP("sem-pivo");
    stop = timestamp();

    printf("X = [");
    for(int i=0; i<matriz3->tam; i++) {
        printf(" %f", matriz3->X[i]);
    }
    printf(" ]\n");

    printf("RESIDUO = [");
    for (int i = 0; i<matriz3->tam; i++) {
        printf(" %f", matriz3->residuo[i]);
    }
    printf(" ]\n");
    printf("TEMPO = %f\n", stop - start);


    LIKWID_MARKER_CLOSE;

    libera(matriz);
    libera(matriz2);
    libera(matriz3);
    return 0;
}
