#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "stack_unbound.h"

// gcc stack_unbound_main.c stack_unbound.c -o stack_unbound

// Ejecutar (para mostrar el race condition):
// while true; do ./stack_unbound; done

#define ELEMENT_COUNT 2000

CStack* root;

void *function1( ) {
  
  for(int i = 0; i < ELEMENT_COUNT; ++i)
  {
    conc_push(root, i);
  }

}

void *function2( ) {
  
  for(int i = 0; i < ELEMENT_COUNT; ++i)
  {
    conc_push(root, i);
  }

}


int main()
{
  root = cstack_init();
  pthread_t one, two; 
  pthread_create(&one, NULL, function1, NULL);  // create thread
  pthread_create(&two, NULL, function2, NULL);

  pthread_join(one, NULL);
  pthread_join(two, NULL);

  int  counter = 0;

  while(!isEmpty(root))
  {
    counter++;
    conc_pop(root);
  }

  printf("counter esperado: %d\n", ELEMENT_COUNT * 2);
  printf("counter final: %d\n", counter);

  stackFree(root);
 
  return 0;
}