/* 
Ej. 5. Implemente el algoritmo de Peterson para solucionar el problema del jard´ın ornamental. Tenga
en cuenta lo discutido sobre barreras de memoria. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#define VISITANTE 10000000

int intenciones[2] = {0};
int turno = 0; // 0 si m1, 1 si m2

int visitantes = 0;

void* molinete1(void * c){
    for(int i = 0;i< VISITANTE;i++){
        intenciones[0] = 1;
        asm("mfence");
        turno = 1;
        while (intenciones[1] && turno)
        {;}
        visitantes++;
        intenciones[0] = 0;
    }
}

void* molinete2(void * c){
    for(int i = 0;i< VISITANTE;i++){
        intenciones[1] = 1;
        asm("mfence");
        turno = 0;
        while (intenciones[0] && !(turno))
        {;}
        visitantes++;
        intenciones[1] = 0;
    }
}

int main(){

pthread_t h1,h2;
pthread_create(&h1,NULL,molinete1,NULL);
pthread_create(&h2,NULL,molinete2,NULL);
pthread_join(h1,NULL);
pthread_join(h2,NULL);

printf("%d\n", visitantes);
return 0;

}