#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#define MUESTRAS 90000000
#define RADIO 100000
#define LADO (2*RADIO)

typedef struct _punto
{
    double x;
    double y;
} punto;

typedef struct _localizacion
{
    int totales;
    int sqr;
    int cir;
} localizacion;


/* void print_hello(void * argv){
    printf("hola mundo\n");
    
}

int main(){
    pthread_t hilo; 
    pthread_create(&hilo,NULL,print_hello,NULL );
    pthread_join(hilo, NULL);
    return 0;
} */

punto *tirar_punto()
{
    punto *punto = malloc(sizeof(punto));
    punto->x = (double) (rand() % LADO);
    punto->y = (double) (rand() % LADO);
    return punto;
}

void localizar_punto(localizacion *l, punto *punto)
{
    l->totales++;
    double distancia = sqrt(pow((punto->x - RADIO),2) + pow((punto->y - RADIO),2));
    if (distancia > (double) RADIO) l->sqr++;
    else l->cir++;
    free(punto);
}

int main(){
    localizacion *l = malloc(sizeof(localizacion));
    l->totales = 0;
    l->sqr = 0;
    l->cir = 0;

    for(int i = 0; i < MUESTRAS; i++){
        punto *p = tirar_punto();
        localizar_punto(l,p);
    }

    float pi = (float) (4 * l->cir) / (float) l->totales;  
    printf("PI = %f\n", pi);

    free(l);
    return 0;
}




