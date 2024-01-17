#ifndef INTERVALOOP_H
#define INTERVALOOP_H

struct intervalo {
    double min;
    double max;
    double erro_absoluto;
    double erro_relativo;
    double ulp;
};
typedef struct intervalo intervalo_t;

intervalo_t multiplica(intervalo_t inter1, intervalo_t inter2);
intervalo_t soma(intervalo_t inter1, intervalo_t inter2);
intervalo_t subtracao(intervalo_t inter1, intervalo_t inter2);
intervalo_t divisao(intervalo_t inter1, intervalo_t inter2);
intervalo_t power(intervalo_t interval, int p);
void transformaIntervalo(intervalo_t *intervalo, double num);
void printaIntervalo(intervalo_t* intervalo);
#endif