#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "channels.h"

// gcc stack_unbound_main.c stack_unbound.c -o stack_unbound

// Ejecutar (para mostrar el race condition):
// while true; do ./stack_unbound; done

#define ELEMENT_COUNT 2000
#define MAX_ELMS 10

channel canal;

void *productor(void *num){
  int i;
  int *numero = (int *) num;
  for (i = 1; i <= 20000; i++)
  {
    channel_write(&canal, i);
    printf("Productor %d: escrito %d\n", *numero, i);
  }
  return NULL;
}

void *consumidor(void *num){
    int *numero = (int *) num;
    while(1)
    {
      int v = channel_read(&canal);
      printf("Consumidor %d: leido %d\n", *numero, v);
    }
}

int main()
{
  channel_init(&canal);

  pthread_t one, two, three, four;
  int uno = 1, dos = 1, tres = 2, cuatro = 2;
  pthread_create(&one, NULL, productor, &uno);  // create thread
  pthread_create(&two, NULL, consumidor, &dos);
  pthread_create(&three, NULL, consumidor, &tres);  // create thread
  pthread_create(&four, NULL, productor, &cuatro);  // create thread

  pthread_join(one, NULL);
  pthread_join(two, NULL);
  pthread_join(three, NULL);
  pthread_join(four, NULL);
 
  return 0;
}