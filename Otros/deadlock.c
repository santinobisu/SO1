#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#define VISITANTE 1000

int visitantes = 0;
pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
void* molinete1(void * c){

    for(int i = 0;i< VISITANTE;i++){
        printf("m1 pide el lock 1 \n");
        pthread_mutex_lock(&lock1);
        printf("m1 pide el lock 2\n");
        pthread_mutex_lock(&lock2);
        printf("m1 suelta el lock 2 \n");
        pthread_mutex_unlock(&lock2);
        printf("m1 suelta el lock 1 \n");
        pthread_mutex_unlock(&lock1);
    }
}

void* molinete2(void * c){

    for(int i = 0;i< VISITANTE;i++){
        printf("m2 pide el lock 2\n");
        pthread_mutex_lock(&lock2);
        printf("m2 pide el lock 1\n");
        pthread_mutex_lock(&lock1);
        printf("m2 suelta el lock 1 \n");
        pthread_mutex_unlock(&lock1);
        printf("m2 suelta el lock 2 \n");
        pthread_mutex_unlock(&lock2);
        
    }
}

int main(){

pthread_t h1,h2;
pthread_create(&h1,NULL,molinete1,NULL);
pthread_create(&h2,NULL,molinete2,NULL);
pthread_join(h1,NULL);
pthread_join(h2,NULL);

pthread_mutex_destroy(&lock1);
pthread_mutex_destroy(&lock2);

printf("%d\n", visitantes);
return 0;

}