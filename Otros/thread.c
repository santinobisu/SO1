#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#define MUESTRAS 90000000
#define RADIO 50
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

typedef struct _localizaciones
{
    localizacion *l[4];
} localizaciones;


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

void *pthread_laburo(void* l){

    for(int i = 0; i < MUESTRAS/4; i++)
    {
        punto *p = tirar_punto();
        localizar_punto(l,p);
    }
    
    return l;
}

int main(){

    localizaciones *localizaciones_general = malloc(sizeof(localizaciones));
    for (int i = 0; i < 4; i++)
    {
        localizaciones_general->l[i] = malloc(sizeof(localizacion));
        localizaciones_general->l[i]->totales = 0;
        localizaciones_general->l[i]->sqr = 0;
        localizaciones_general->l[i]->cir = 0;
    }
    localizacion *l = malloc(sizeof(localizacion));
    l->totales = 0;
    l->sqr = 0;
    l->cir = 0;
    pthread_t hilo1;
    pthread_t hilo2;
    pthread_t hilo3;
    pthread_t hilo4;
    pthread_create(&hilo1,NULL,pthread_laburo,localizaciones_general->l[0]);
    pthread_create(&hilo2,NULL,pthread_laburo,localizaciones_general->l[1]);
    pthread_create(&hilo3,NULL,pthread_laburo,localizaciones_general->l[2]);
    pthread_create(&hilo4,NULL,pthread_laburo,localizaciones_general->l[3]);
    
    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);
    pthread_join(hilo3, NULL);
    pthread_join(hilo4, NULL);

    l->totales += localizaciones_general->l[0]->totales;
    l->totales += localizaciones_general->l[1]->totales;
    l->totales += localizaciones_general->l[2]->totales;
    l->totales += localizaciones_general->l[3]->totales;

    l->sqr += localizaciones_general->l[0]->sqr;
    l->sqr += localizaciones_general->l[1]->sqr;
    l->sqr += localizaciones_general->l[2]->sqr;
    l->sqr += localizaciones_general->l[3]->sqr;

    l->cir += localizaciones_general->l[0]->cir;
    l->cir += localizaciones_general->l[1]->cir;
    l->cir += localizaciones_general->l[2]->cir;
    l->cir += localizaciones_general->l[3]->cir;

    float pi = (float) (4 * l->cir) / (float) l->totales;  
    printf("PI = %f\n", pi);

    for (int i = 0; i < 4; i++)
    {
        free(localizaciones_general->l[i]);
    }
    free(localizaciones_general);
    free(l);

    return 0;
}




