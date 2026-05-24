/* 
Ej. 6. Considere el problema del jard´ın ornamental en un sistema con un unico procesador.

a) ¿Sigue habiendo un problema de concurrencia? Justifique. 
No, pues el algoritmo de Petersen asegura que (en un sistema con un unico procesador, y con dos hilos) no haya problemas de concurrencia.

b) Si implementa el algoritmo de Peterson, ¿son necesarias las barreras de memoria?
No, al tener un solo procesador no es necesario.

c) Si el incremento se hace con la instruccion incl de x86, ¿hay problema? Puede aprovechar la
siguiente funcion:
static inline void incl(int *p) {
asm("incl %0" : "+m"(*p) : : "memory");
}
No, no sigue habiendo un problema de concurrencia pues la instrucción es atómica en asm.

d) ¿Qu´e pasa con la implementacion con incl al tener mas de un procesador?
Deja de funcionar, debido al store buffering que ocurre en los nucleos.

e) Repita el experimento con esta versi´on de incl:
static inline void incl(int *p) {
asm("lock; incl %0" : "+m"(*p) : : "memory");
} 
En esta versión el código funciona sin problemas al tener tanto uno como más procesadores.
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#define VISITANTE 10000000

static inline void incl(int *p) {
asm("incl %0" : "+m"(*p) : : "memory");}


static inline void incll(int *p) {
asm("lock; incl %0" : "+m"(*p) : : "memory");} 


int intenciones[2] = {0};
int turno = 0; // 0 si m1, 1 si m2

int visitantes = 0;

void* molinete1(void * c){
    for(int i = 0;i< VISITANTE;i++){
        incll(&visitantes);
    }
}

void* molinete2(void * c){
    for(int i = 0;i< VISITANTE;i++){
        incll(&visitantes);
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