#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#define VISITANTE 1000000

int visitantes = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
void* molinete1(void * c){
    int i;
    for(int i = 0;i< VISITANTE;i++){
        pthread_mutex_lock(&lock);
        visitantes++;
        pthread_mutex_unlock(&lock);
    }
}

void* molinete2(void * c){
    int i;
    for(int i = 0;i< VISITANTE;i++){
        pthread_mutex_lock(&lock);
        visitantes++;
        pthread_mutex_unlock(&lock);
        
    }
}

int main(){

pthread_t h1,h2;
pthread_create(&h1,NULL,molinete1,NULL);
pthread_create(&h2,NULL,molinete2,NULL);
pthread_join(h1,NULL);
pthread_join(h2,NULL);

pthread_mutex_destroy(&lock);

printf("%d\n", visitantes);
return 0;

}