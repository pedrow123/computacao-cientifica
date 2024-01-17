#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "matriz.h"

// Cria uma estrutura matriz_t dinamicamente e inicializa os valores com 0
matriz_t *criaMatriz(int tam) {
    matriz_t * matriz = malloc(sizeof(matriz_t));
    if(!matriz)
        return NULL;

    // Inicializa A
    matriz->A = malloc(sizeof(double*)*tam);
    if(!matriz->A)
        return NULL;

    for(int i=0; i<tam; i++) {
        matriz->A[i] = malloc(sizeof(double)*tam);
        if(!matriz->A[i])
            return NULL;
    }    
    for(int i=0; i<tam; i++) {
        for(int j=0; j<tam; j++) {
            matriz->A[i][j] = 0;
        }
    }

    // Inicializa B, X e resíduo
    matriz->B = malloc(sizeof(double)*tam);
    if(!matriz->B)
        return NULL;
    matriz->X = malloc(sizeof(double)*tam);
    if(!matriz->X)
        return NULL;
    matriz->residuo = malloc(sizeof(double)*tam);
    if(!matriz->residuo)
        return NULL;

    for(int i=0; i<tam; i++) {
        matriz->B[i] = 0;
        matriz->X[i] = 0;
        matriz->residuo[i] = 0;
    }

    // Inicializa outras variáveis
    matriz->tam = tam;
    return matriz;

}

// Recebe a entrada formatada da matriz
void recebeEntrada(matriz_t *matriz) {
    int returnScanf = 0;
    for(int i=0; i<matriz->tam; i++) {
        for(int j=0; j<matriz->tam; j++) {
            returnScanf = scanf("%lf", &matriz->A[i][j]);
        }
        returnScanf = scanf("%lf", &matriz->B[i]);
    }
}

// Printa a matriz
void printaMatriz(matriz_t *matriz) {
    for(int i=0; i<matriz->tam; i++) {
        for(int j=0; j<matriz->tam; j++)
            printf("%f ", matriz->A[i][j]);
        printf("| %f\n", matriz->B[i]);
    }
}

// Usa o método da retrossubistituição em uma matriz após a elminação de gauss
void retrossubs(matriz_t *matriz) {
    for(int i=matriz->tam-1; i>=0; i--) {
        matriz->X[i] = matriz->B[i];
        for(int j=matriz->tam-1; j>i; j--) {
            matriz->X[i] -= matriz->X[j]*matriz->A[i][j];
        }
        matriz->X[i] = matriz->X[i] / matriz->A[i][i];
    }
}

// Função auxiliar do pivoteamento parcial para encontrar o máximo
int encontraMaxParcial(matriz_t *matriz, int i) {
    int iMax = i;
    for(int j=i+1; j<matriz->tam; j++)
        if(abs(matriz->A[j][i]) > abs(matriz->A[iMax][i]))
            iMax = j;
    return iMax;
}

// Função auxiliar do pivoteamento total para encontrar o máximo
indice_t encontraMaxTotal(matriz_t *matriz, int i) {
    indice_t indice;
    indice.i = i;
    indice.j = i;
    for(int j=i; j<matriz->tam; j++) {
        for(int k=i; k<matriz->tam; k++) {
            if(matriz->A[j][k] > matriz->A[indice.i][indice.j]) {
                indice.i = j;
                indice.j = k;
            }
        }
    }
    return indice;
}

// Função auxiliar do pivoteamento total para trocar as colunas
void trocaColunas(matriz_t *matriz, int jMax, int j) {
    // Troca colunas
    double aux = 0;
    for(int i=0; i<matriz->tam; i++) {
        aux = matriz->A[i][jMax];
        matriz->A[i][jMax] = matriz->A[i][j];
        matriz->A[i][j] = aux;
    }
}

// Função auxiliar do pivoteamento parcial e total para trocar as linhas
void trocaLinhas(matriz_t *matriz, int iMax, int i) {
    if(iMax == i)
        return;
    double aux = 0;
    for(int j=0; j<matriz->tam; j++) {
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

// Pivoteamento total de uma matriz a partir de um índice i
void pivoteamentoTotal(matriz_t *matriz, int i) {
    indice_t indiceMax = encontraMaxTotal(matriz, i);
    trocaColunas(matriz, indiceMax.j, i);
    trocaLinhas(matriz, indiceMax.i, i);
}

// Aplica o método da eliminação gaussiana
void eliminacaoGauss(matriz_t *matriz) {
    double m = 0;
    int iMax = 0;
    // Para cada linha
    for(int i=0; i<matriz->tam; i++) {
        pivoteamentoParcial(matriz, i);
        // Para cada linha subsequente 
        for(int k=i+1; k<matriz->tam; k++) {
            m = matriz->A[k][i] / matriz->A[i][i];   // m retirado da fórmula para evitar divisões
            matriz->A[k][i] = 0;
            // Para cada coluna da linha subsequente
            for(int j=i+1; j<matriz->tam; j++)
                matriz->A[k][j] -= m*matriz->A[i][j];
            matriz->B[k] -= m*matriz->B[i];
        }
    }
}
// Aplica o método da eliminação gaussiana sem o multiplicador
void eliminacaoGaussSemMultiplicador(matriz_t *matriz) {
    double m = 0;
    int iMax = 0;
    // Para cada linha
    for(int i=0; i<matriz->tam; i++) {
        pivoteamentoParcial(matriz, i);
        // Para cada linha subsequente 
        for(int k=i+1; k<matriz->tam; k++) {
            // Para cada coluna da linha subsequente
            for(int j=i+1; j<matriz->tam; j++)
                matriz->A[k][j] = matriz->A[k][j]*matriz->A[i][i] - matriz->A[i][j]*matriz->A[k][i];
            matriz->B[k] = matriz->B[k]*matriz->A[i][i] - matriz->A[k][i]*matriz->B[i];
	    matriz->A[k][i] = 0;
        }
    }
}

// Divide a linha do pivô pelo próprio pivô
void divisaoPivo(matriz_t *matriz, int indice) {
    double pivo = matriz->A[indice][indice];
    for(int j=indice; j<matriz->tam; j++)
        matriz->A[indice][j] /= pivo; 
    matriz->B[indice] /= pivo;
}

// AplicIa o método da eliminação gaussiana sem o multiplicador
void eliminacaoGaussSemPivoteamento(matriz_t *matriz) {
    double m = 0;
    // Para cada linha
    for(int i=0; i<matriz->tam; i++) {
        divisaoPivo(matriz, i);
        // Para cada linha subsequente 
        for(int k=i+1; k<matriz->tam; k++) {
            m = matriz->A[k][i] / matriz->A[i][i];   // m retirado da fórmula para evitar divisões
            matriz->A[k][i] = 0;
            // Para cada coluna da linha subsequente
            for(int j=i+1; j<matriz->tam; j++)
                matriz->A[k][j] -= m*matriz->A[i][j];
            matriz->B[k] -= m*matriz->B[i];
        }
    }
}

// Calcula o resíduo de uma matriz
void calculaResiduo(matriz_t *matriz) {
    double valor = 0;
    for(int i=0; i<matriz->tam; i++) {
        for(int j=0; j<matriz->tam; j++) {
            valor += matriz->A[i][j]*matriz->X[j]; 
        }
        matriz->residuo[i] = valor - matriz->B[i];
        valor = 0;
    }
}

void copiaMatriz(matriz_t *m1, matriz_t *m2) {
    for(int i=0; i<m1->tam; i++) {
        for(int j=0; j<m1->tam; j++) {
	    m2->A[i][j] = m1->A[i][j];
	}
	m2->B[i] = m1->B[i];
	m2->X[i] = m1->B[i];
	m2->residuo[i] = m1->residuo[i];
    }
}

void libera(matriz_t *matriz) {
    for(int i=0; i<matriz->tam; i++){
	free(matriz->A[i]);
    }
    free(matriz->A);
    free(matriz->B);
    free(matriz->X);
    free(matriz->residuo);
    free(matriz);
}
