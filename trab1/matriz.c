#include "matriz.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Cria uma estrutura matriz_t dinamicamente e inicializa os valores com 0
matriz_t *criaMatriz(int tam) {
    matriz_t *matriz = malloc(sizeof(matriz_t));
    if (!matriz) return NULL;

    // Inicializa A
    matriz->A = malloc(sizeof(intervalo_t *) * tam);
    if (!matriz->A) return NULL;

    for (int i = 0; i < tam; i++) {
        matriz->A[i] = malloc(sizeof(intervalo_t) * tam);
        if (!matriz->A[i]) return NULL;
    }

    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            transformaIntervalo(&matriz->A[i][j], 0);
        }
    }

    // Inicializa B, X e resíduo
    matriz->B = malloc(sizeof(intervalo_t) * tam);
    if (!matriz->B) return NULL;
    matriz->X = malloc(sizeof(intervalo_t) * tam);
    if (!matriz->X) return NULL;
   
    for (int i = 0; i < tam; i++) {
        transformaIntervalo(&matriz->B[i], 0);
        transformaIntervalo(&matriz->X[i], 0);
    }

    // Inicializa outras variáveis
    matriz->tam = tam;
    return matriz;
}

// Printa a matriz
void printaMatriz(matriz_t *matriz) {
    for (int i = 0; i < matriz->tam; i++) {
        for (int j = 0; j < matriz->tam; j++) printaIntervalo(&matriz->A[i][j]);
        printf("| ");
        printaIntervalo(&matriz->B[i]);
        printf("\n");
    }
}

// Usa o método da retrossubistituição em uma matriz após a elminação de gauss
void retrossubs(matriz_t *matriz) {
    for (int i = matriz->tam - 1; i >= 0; i--) {
        matriz->X[i] = matriz->B[i];
        for (int j = matriz->tam - 1; j > i; j--) {
            // matriz->X[i] -= matriz->X[j]*matriz->A[i][j];
            matriz->X[i] = subtracao(matriz->X[i],
                                     multiplica(matriz->X[j], matriz->A[i][j]));
        }
        // matriz->X[i] = matriz->X[i] / matriz->A[i][i];
        matriz->X[i] = divisao(matriz->X[i], matriz->A[i][i]);
    }
}

// Função auxiliar do pivoteamento parcial para encontrar o máximo
int encontraMaxParcial(matriz_t *matriz, int i) {
    int iMax = i;
    for (int j = i + 1; j < matriz->tam; j++)
        // if(abs(matriz->A[j][i]) > abs(matriz->A[iMax][i]))
        if (abs(matriz->A[j][i].max) > abs(matriz->A[iMax][i].max)) iMax = j;
    return iMax;
}

// Função auxiliar do pivoteamento parcial e total para trocar as linhas
void trocaLinhas(matriz_t *matriz, int iMax, int i) {
    if (iMax == i) return;
    intervalo_t aux;
    for (int j = 0; j < matriz->tam; j++) {
        aux = matriz->A[i][j];
        matriz->A[i][j] = matriz->A[iMax][j];
        matriz->A[iMax][j] = aux;
    }
    aux = matriz->B[iMax];
    matriz->B[iMax] = matriz->B[i];
    matriz->B[i] = aux;
}

// Pivoteamento parcial de uma matriz a partir de um índice i
void pivoteamentoParcial(matriz_t *matriz, int i) {
    int iMax = encontraMaxParcial(matriz, i);
    trocaLinhas(matriz, iMax, i);
}

// Aplica o método da eliminação gaussiana
void eliminacaoGauss(matriz_t *matriz) {
    intervalo_t m, sub, mul;
    int iMax = 0;
    // Para cada linha
    for (int i = 0; i < matriz->tam; i++) {
        pivoteamentoParcial(matriz, i);
        // printaMatriz(matriz);
        // printf("\n");   
        // printaMatriz(matriz);
        // Para cada linha subsequente
        for (int k = i + 1; k < matriz->tam; k++) {
            m = divisao(matriz->A[k][i], matriz->A[i][i]);
            matriz->A[k][i].max = 0;
            matriz->A[k][i].min = 0;
            // Para cada coluna da linha subsequente
            for (int j = i + 1; j < matriz->tam; j++) {
                mul = multiplica(matriz->A[i][j], m);
                sub = subtracao(matriz->A[k][j], mul);
                matriz->A[k][j] = sub;
            }
            matriz->B[k] = subtracao(matriz->B[k], multiplica(matriz->B[i], m));
        }
    }
}

void criaSL(pontos_t *xy, matriz_t *SL, int k, int n) {
    intervalo_t aux;
    intervalo_t aux2;
    for (int i = 0; i < n + 1; i++) {
        for (int j = 0; j < n + 1; j++) {
            for (int sum = 0; sum < k; sum++) {
                transformaIntervalo(&aux, xy[sum].x);
                aux = multiplica(power(aux, i), power(aux, j));
                SL->A[i][j] = soma(SL->A[i][j], aux);
                // SL->A[i][j] += pow(xy[sum].x, i) * pow(xy[sum].x, j);
            }
        }
        for (int sum = 0; sum < k; sum++) {
            transformaIntervalo(&aux, xy[sum].x);
            transformaIntervalo(&aux2, xy[sum].y);
            aux = multiplica(aux2, power(aux, i));
            SL->B[i] = soma(SL->B[i], aux);
            // SL->B[i] += xy[sum].y * pow(xy[sum].x, i);
        }
    }
}

// Calcula o resíduo de uma matriz
intervalo_t* calculaResiduo(matriz_t *matriz, pontos_t *xy, int k){
    intervalo_t aux, fx;
    intervalo_t *ret = malloc(sizeof(intervalo_t)*k);
    if(!ret)
        return NULL;
    
    for (int i = 0; i < k; i++) {
        transformaIntervalo(&aux, xy[i].x);
        fx.max = 0;
        fx.min = 0;
        for (int j = 0; j < matriz->tam; j++) {
            fx = soma(fx, multiplica(matriz->X[j], power(aux, j)));
        }
        transformaIntervalo(&aux, xy[i].y);
        ret[i] = subtracao(aux, fx);
    }
    return ret;
}

void libera(matriz_t *matriz, intervalo_t* residuo) {
    for (int i = 0; i < matriz->tam; i++) {
        free(matriz->A[i]);
    }
    free(matriz->A);
    free(matriz->B);
    free(matriz->X);
    free(residuo);
    free(matriz);
}
