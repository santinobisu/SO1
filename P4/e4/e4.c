/*
Ej. 4 (Primalidad). Escriba utilizando OpenMP una funci´on que verifique si un entero es primo
(buscando divisores entre 2 y √N). Su soluci´on deber´ıa andar igual o m´as r´apido que una versi´on secuencial
que “corta” apenas encuentra un divisor. Escriba su funci´on para tomar un long, i.e. un entero de 64
bits1, y aseg´urese de probarla con n´umeros grandes (incluyendo primos, semiprimos, y pares).
*/


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>


int es_primo(long int n)
{
    double sqrn = sqrt(n);
    int bandera = 1;
    
    #pragma omp parallel for reduction(&&: bandera)
    for(int i = 2; i < (int)sqrn; i++){
        if( n % i == 0) bandera = 0;
    }

    return bandera;
}


int es_primo_secuencial(long int n)
{
    double sqrn = sqrt(n);
    int and = 1;
    
    for(int i = 2; i < (int)sqrn; i++){
        if( n % i == 0) and = 0;
    }

    return and;
}



int main ()
{
    long int num = 9999900000099967; 
    double inicio, fin, t_sec, t_par;

    printf("--- COMPARATIVA DE FUERZA BRUTA PURA ---\n");
    printf("Analizando el primo: %ld\n\n", num);

    // Medición Secuencial
    inicio = omp_get_wtime();
    int r_sec = es_primo_secuencial(num);
    fin = omp_get_wtime();
    t_sec = fin - inicio;
    printf("[Secuencial] Resultado: %d | Tiempo: %f seg\n", r_sec, t_sec);

    // Medición Paralela
    inicio = omp_get_wtime();
    int r_par = es_primo(num);
    fin = omp_get_wtime();
    t_par = fin - inicio;
    printf("[Paralelo]   Resultado: %d | Tiempo: %f seg\n\n", r_par, t_par);

    return 0;
}