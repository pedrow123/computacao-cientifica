#ifndef TABELA_H
#define TABELA_H

typedef struct tabela{
    int n;
    double *x;
    double *y;
} tabela_h;

tabela_h *cria_tabela(int n);
void le_pontos(tabela_h *tabela);
void imprime_pontos(tabela_h *tabela);

#endif