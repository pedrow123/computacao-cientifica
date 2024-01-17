#include <stdio.h>
#include <stdlib.h>
#include "tabela.h"

// Cria uma estrutura tabela_h 
tabela_h *cria_tabela(int n) {
    tabela_h *tabela = malloc(sizeof(tabela_h));
    if(!tabela)
        return NULL;
    
    tabela->n = n;
    tabela->x = malloc(sizeof(double)*n);
    if(!tabela->x)
        return NULL;
    tabela->y = malloc(sizeof(double)*n);
    if(!tabela->y)
        return NULL;
    
    return tabela;
}

// Lê os pontos passados pelo stdin
void le_pontos(tabela_h *tabela) {
    int return_scanf = 0;
    for(int i=0; i<tabela->n; i++)
        return_scanf = fscanf(stdin, "%lf %lf", &tabela->x[i], &tabela->y[i]);
}

// Imprime pontos da tabela
void imprime_pontos(tabela_h *tabela) {
    for(int i=0; i<tabela->n; i++)
        printf("(%lf, %lf)\n", tabela->x[i], tabela->y[i]);
}
