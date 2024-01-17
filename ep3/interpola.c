#include <stdio.h>
#include <stdlib.h>
#include "tabela.h"
#include <sys/time.h>
#include "likwid.h"

// Calcula o tempo atual em milissegundos
double timestamp(void){
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return((double)(tp.tv_sec+tp.tv_usec/1000000.0))*1000;
}

// Método de interpolação de Lagrange
double lagrange(double xe, tabela_h *tabela) {
    double resultado = 0.0;

    for (int i = 0; i < tabela->n; i++) {
        double mult = tabela->y[i];
        for (int j = 0; j < tabela->n; j++)
            if (j != i)
                mult *= (xe - tabela->x[j]) / (tabela->x[i] - tabela->x[j]);
        resultado += mult;
    }

    return resultado;
}

// Calcula o D de cada parte da interpolação de Newton
double *calcular_d(int n, double *x, double *y, double *diff) {
    for (int j = 1; j < n; j++)
        for (int i = n - 1; i >= j; i--)
            diff[i] = (diff[i] - diff[i - 1]) / (x[i] - x[i - j]);
    return diff;
}

// Método de interpolação de Newton
double newton(double xe, tabela_h *tabela) {
    double *diff = malloc(tabela->n * sizeof(double));
    if (diff == NULL) {
        perror("Erro ao alocar memória");
        exit(1);
    }

    for (int i = 0; i < tabela->n; i++)
        diff[i] = tabela->y[i];

    calcular_d(tabela->n, tabela->x, tabela->y, diff);
    
    double resultado = diff[0];
    double mult = 1.0;

    for (int i = 1; i < tabela->n; i++) {
        mult *= (xe - tabela->x[i - 1]);
        resultado += diff[i] * mult;
    }
    free(diff); 

    return resultado;
}

int main(int argc, char** argv) {
    // Caso falta argumento xe
    if(!argv[1]) {
        printf("Falta argumento ponto xe!\n./interpola xe < entrada.in\n");
        return 0;
    }

    LIKWID_MARKER_INIT;
    int n;
    double start, stop, lagrange_time, newton_time = 0.0;
    int return_scanf = 0;
    double xe;
    xe = atoi(argv[1]);
    return_scanf = fscanf(stdin, "%d", &n); // Lê quantidade de pontos
    tabela_h *tabela = cria_tabela(n);
    le_pontos(tabela);
    // imprime_pontos(tabela);

    // Calcula e imprime resultados
    LIKWID_MARKER_START("lagrange");
    start = timestamp();
    double resultado = lagrange(xe, tabela);
    stop = timestamp();
    lagrange_time = stop - start;
    LIKWID_MARKER_STOP("lagrange");

    LIKWID_MARKER_START("newton");
    start = timestamp();
    double resultado2 = newton(xe, tabela);
    stop = timestamp();
    newton_time = stop - start;
    LIKWID_MARKER_STOP("newton");
    
    printf("Resultado Lagrange: %lf\n", resultado);
    printf("Resultado Newton: %lf\n", resultado2);
    printf("Tempo Lagrange: %.7lf\n", lagrange_time);
    printf("Tempo Newton: %.7lf\n", newton_time);

    LIKWID_MARKER_CLOSE;
    return 0;
}
