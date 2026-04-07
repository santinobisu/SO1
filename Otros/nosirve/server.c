#include "calculadora.h"
#include <stdio.h>
#include <stdlib.h>

int * sumar_1_svc (operandos *argp, struct svc_req *rqstp){

    static int resultado; 
    printf("servidor: sumar %d + %d\n", argp->a , argp->b);
    resultado = argp->a + argp->b;
    return &resultado;
}


