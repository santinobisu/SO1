#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "stack_unbound.h"
#include <semaphore.h>


// gcc stack_unbound_main.c stack_unbound.c -o stack_unbound

// Ejecutar (para mostrar el race condition):
// while true; do ./stack_unbound; done

#define ELEMENT_COUNT 2000
#define MAX_ELMS 10

sem_t s_lugares;
sem_t s_ocupados;


CStack* root;

void *productor(){
  int i;
  for (i = 1; i <= 20000; i++)
  {
    sem_wait(&s_lugares);
    conc_push(root, i);
    printf("Productor: pusheado %d\n", i);
    sem_post(&s_ocupados);
  }
}

void *consumidor(){
    while(1)
    {
        sem_wait(&s_ocupados);
        int v = conc_pop(root);
        printf("Consumidor: poppeado %d\n", v);
        sem_post(&s_lugares);
    }
}


int main()
{
  root = cstack_init();

  sem_init(&s_lugares, 0, MAX_ELMS);
  sem_init(&s_ocupados, 0, 0);

  pthread_t one, two, three, four;
  pthread_create(&one, NULL, productor, NULL);  // create thread
  pthread_create(&two, NULL, consumidor, NULL);
  pthread_create(&three, NULL, consumidor, NULL);  // create thread
  pthread_create(&four, NULL, productor, NULL);  // create thread

  pthread_join(one, NULL);
  pthread_join(two, NULL);
  pthread_join(three, NULL);
  pthread_join(four, NULL);

  stackFree(root);
 
  return 0;
}