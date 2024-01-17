#include <fenv.h>
#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "intervaloOp.h"

intervalo_t multiplica(intervalo_t inter1, intervalo_t inter2) {
    intervalo_t res;
    fesetround(FE_DOWNWARD);
    res.min = fmin(fmin(inter1.min * inter2.min, inter1.min * inter2.max),
                    fmin(inter1.max * inter2.min, inter1.max * inter2.max));
    fesetround(FE_UPWARD);
    res.max = fmax(fmax(inter1.min * inter2.min, inter1.min * inter2.max),
                    fmax(inter1.max * inter2.min, inter1.max * inter2.max));
    
    // printf("MULT = ");
    // printaIntervalo(&res);
    // printf("\n");
    return res;
}

intervalo_t soma(intervalo_t inter1, intervalo_t inter2) {
    intervalo_t res;
    fesetround(FE_DOWNWARD);
    res.min = inter1.min + inter2.min;
    fesetround(FE_UPWARD);
    res.max = inter1.max + inter2.max;
    // printf("SOMA = ");
    // printaIntervalo(&res);
    // printf("\n");
    return res;
}

intervalo_t subtracao(intervalo_t inter1, intervalo_t inter2) {
    intervalo_t res;
    double aux;
    fesetround(FE_DOWNWARD);
    res.min = inter1.min - inter2.max;
    fesetround(FE_UPWARD);
    res.max = inter1.max - inter2.min;
    // printf("SUB = ");
    // printaIntervalo(&res);
    // printf("\n");
    return res;
}

intervalo_t divisao(intervalo_t inter1, intervalo_t inter2) {
    intervalo_t result;
    double aux;
    if (inter2.min == 0.0 || inter2.max == 0.0) {
        result.min = -INFINITY;
        result.max = INFINITY;
    } else {
        result.min = 1 / inter2.max;
        result.max = 1 / inter2.min;
        result = multiplica(inter1, result);
    }
    // printf("DIV = ");
    // printaIntervalo(&result);
    // printf("\n");
    return result;
}

intervalo_t power(intervalo_t interval, int p) {
    intervalo_t result;
    double aux = 0;
    if (p == 0) {
        result.min = 1;
        result.max = 1;
    } else if (p % 2 == 1) {
        result.min = pow(interval.min, p);
        result.max = pow(interval.max, p);
    } else if (p % 2 == 0 && interval.min >= 0) {
        result.min = pow(interval.min, p);
        result.max = pow(interval.max, p);
    } else if (p % 2 == 0 && interval.max < 0) {
        result.min = pow(interval.max, p);
        result.max = pow(interval.min, p);
    } else if (p % 2 == 0 && interval.min < 0 && interval.max >= 0) {
        result.min = 0;
        result.max = fmax(pow(interval.min, p), pow(interval.max, p));
    }

    // printf("POWER = ");
    // printaIntervalo(&result);
    // printf("\n");
    return result;
}

void transformaIntervalo(intervalo_t *intervalo, double num) {
    intervalo->min = nextafter(num, -INFINITY);
    intervalo->max = nextafter(num, +INFINITY);
}

void printaIntervalo(intervalo_t* intervalo) {
    printf("[%1.8e, %1.8e]", intervalo->min, intervalo->max);
}