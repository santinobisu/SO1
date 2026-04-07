#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#define VISITANTE 1000000

int visitantes = 0;

void* molinete(void * c){
    int i;
    for(int i = 0;i< VISITANTE;i++){
        visitantes++;
        
    }
}

int main(){

pthread_t h1,h2;
pthread_create(&h1,NULL,molinete,NULL);
pthread_create(&h2,NULL,molinete,NULL);
pthread_join(h1,NULL);
pthread_join(h2,NULL);

printf("%d\n", visitantes);
return 0;

}