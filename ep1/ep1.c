#include <fenv.h>
#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct intervalo {
    float min;
    float max;
    float erro_absoluto;
    float erro_relativo;
    float ulp;
};
typedef struct intervalo intervalo_t;

intervalo_t multiplica(intervalo_t inter1, intervalo_t inter2) {
    intervalo_t res;
    fesetround(FE_DOWNWARD);
    res.min = fminf(fminf(inter1.min * inter2.min, inter1.min * inter2.max),
                    fminf(inter1.max * inter2.min, inter1.max * inter2.max));
    fesetround(FE_UPWARD);
    res.max = fmaxf(fmaxf(inter1.min * inter2.min, inter1.min * inter2.max),
                    fmaxf(inter1.max * inter2.min, inter1.max * inter2.max));
    return res;
}

intervalo_t soma(intervalo_t inter1, intervalo_t inter2) {
    intervalo_t res;
    fesetround(FE_DOWNWARD);
    res.min = inter1.min + inter2.min;
    fesetround(FE_UPWARD);
    res.max = inter1.max + inter2.max;
    return res;
}

intervalo_t subtracao(intervalo_t inter1, intervalo_t inter2) {
    intervalo_t res;
    fesetround(FE_DOWNWARD);
    res.min = inter1.min - inter2.min;
    fesetround(FE_UPWARD);
    res.max = inter1.max - inter2.max;
    return res;
}

intervalo_t divisao(intervalo_t inter1, intervalo_t inter2) {
    intervalo_t aux;
    if (inter2.min == 0.0 || inter2.max == 0.0) {
        aux.min = -INFINITY;
        aux.max = INFINITY;
    } else {
        aux.min = 1 / inter2.max;
        aux.max = 1 / inter2.min;
        aux = multiplica(inter1, aux);
    }
    return aux;
}

intervalo_t performOperation(intervalo_t a, char operator, intervalo_t b) {
    intervalo_t result;
    switch (operator) {
        case '+':
            result = soma(a, b);
            break;
        case '-':
            result = subtracao(a, b);
            break;
        case '*':
            result = multiplica(a, b);
            break;
        case '/':
            result = divisao(a, b);
            break;
    }
    result.erro_absoluto = result.max-result.min;
    result.erro_relativo = (result.max-result.min)/result.min;
    return result;
}

int main() {

    float x1, x2, x3, x4, x5;
    char op[4];

    intervalo_t result, result_aux;
    intervalo_t intervalos[5];

    scanf("%f %c %f %c %f %c %f %c %f", &x1, &op[0], &x2, &op[1], &x3, &op[2], &x4, &op[3],
          &x5);

    fesetround(FE_DOWNWARD); // Garante que o arredondamento é para cima
    intervalos[0].min = nextafterf(x1, -INFINITY);
    intervalos[0].max = x1;

    intervalos[1].min = nextafterf(x2, -INFINITY);
    intervalos[1].max = x2;

    intervalos[2].min = nextafterf(x3, -INFINITY);
    intervalos[2].max = x3;

    intervalos[3].min = nextafterf(x4, -INFINITY);
    intervalos[3].max = x4;

    intervalos[4].min = nextafterf(x5, -INFINITY);
    intervalos[4].max = x5;

    // 2.347e-40 * 0.001 + 1.1e+10 - 0.75e-39 / 0.0

    printf("======= RESULTADOS =======\n");
    result = intervalos[0];
    int ulp = 0;
    float aux = 0.0;
    for(int i=0; i<4; i++) {
        ulp = 0;
        result_aux = result;
        result = performOperation(result, op[i], intervalos[i+1]);
        printf("%d:\n", i+1);
        printf("[%1.8e, %1.8e] %c [%1.8e, %1.8e] = \n[%1.8e, %1.8e]\n", result_aux.min, result_aux.max, op[i], intervalos[i+1].min, intervalos[i+1].max, result.min, result.max);
        printf("EA: %1.8e; ER: %1.8e; ULP: ", result.erro_absoluto, result.erro_relativo);
        aux = nextafterf(result.min, INFINITY);
        while(aux < result.max && !isinf(result.erro_absoluto)) {
            ulp++;
            aux = nextafterf(aux, INFINITY);
        }
        printf("%d\n\n", ulp);
    }
    return 0;
}
