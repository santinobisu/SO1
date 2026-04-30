#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#define VISITANTE 5000
#define NUM_HILOS 10
int visitantes = 0;
int numero[NUM_HILOS] = {0};
int eligiendo[NUM_HILOS] = {0};

int buscarMax(int arr[])
{
    int max = 0;
    for (int i = 0; i < NUM_HILOS; i++) if (arr[i] > max) max = arr[i];
    return max;
}

void lock(int i)
{
    eligiendo[i] = 1;
    numero[i] = 1 + buscarMax(numero);
    eligiendo[i] = 0;

    for (int j = 0; j < NUM_HILOS; j++)
    {
        while(eligiendo[j]);
        while((numero[j] != 0) && ((numero[j] < numero[i]) || (numero[j] == numero[i]) && (j < i)));
    }
}

void unlock(int i)
{
    numero[i] = 0;
}

void* molinete(void *carg){
    int *c = carg;
    for(int i = 0; i < VISITANTE; i++){
        lock(*c);
        visitantes++;
        unlock(*c);
    }
    return;
}


int main(){
    
    
    pthread_t hilo1, hilo2, hilo3, hilo4, hilo5, hilo6, hilo7, hilo8, hilo9, hilo10;
    int num1= 0, num2= 1, num3= 2, num4= 3, num5= 4, num6= 5, num7= 6, num8= 7, num9= 8, num10= 9;
    pthread_create(&hilo1, NULL, molinete, &num1);
    pthread_create(&hilo2, NULL, molinete, &num2);
    pthread_create(&hilo3, NULL, molinete, &num3);
    pthread_create(&hilo4, NULL, molinete, &num4);
    pthread_create(&hilo5, NULL, molinete, &num5);
    pthread_create(&hilo6, NULL, molinete, &num6);
    pthread_create(&hilo7, NULL, molinete, &num7);
    pthread_create(&hilo8, NULL, molinete, &num8);
    pthread_create(&hilo9, NULL, molinete, &num9);
    pthread_create(&hilo10, NULL, molinete, &num10);

    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);
    pthread_join(hilo3, NULL);
    pthread_join(hilo4, NULL);
    pthread_join(hilo5, NULL);
    pthread_join(hilo6, NULL);
    pthread_join(hilo7, NULL);
    pthread_join(hilo8, NULL);
    pthread_join(hilo9, NULL);
    pthread_join(hilo10, NULL);


    printf("%d\n", visitantes);
    return 0;

}