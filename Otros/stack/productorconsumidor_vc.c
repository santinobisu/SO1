#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "stack_unbound.h"


// gcc stack_unbound_main.c stack_unbound.c -o stack_unbound

// Ejecutar (para mostrar el race condition):
// while true; do ./stack_unbound; done

#define ELEMENT_COUNT 2000
#define MAX_ELMS 10

pthread_cond_t cond_non_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_non_empty = PTHREAD_COND_INITIALIZER;
int i = 0;
CStack* root;

void *productor(){
  while(1)
  {
    pthread_mutex_lock(&(root->lock));
    if (root->c_elms > MAX_ELMS) exit(1);
    while(root->c_elms == MAX_ELMS)
        pthread_cond_wait(&cond_non_full, &(root->lock));
    conc_push(root, i);
    printf("Productor: pusheado el valor %d\n", i);
    i++;
    pthread_mutex_unlock(&(root->lock));
    pthread_cond_signal(&cond_non_empty);
  }

}

void *consumidor(){
  while(1)
  {
    pthread_mutex_lock(&(root->lock));
    while(root->c_elms == 0)
        pthread_cond_wait(&cond_non_empty, &(root->lock));
    int j = conc_pop(root);
    i--;
    printf("Consumidor: poppeado el valor %d\n", j);
    pthread_mutex_unlock(&(root->lock));
    pthread_cond_signal(&cond_non_full);
  }
  
}


int main()
{
  root = cstack_init();
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