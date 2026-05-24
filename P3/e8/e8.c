/*
Ej. 8 (Cola Lock-Free, Lamport). El siguiente fragmento implementa una cola concurrente entre
dos procesos. La variable s mantiene la cantidad de elementos escritos y r los le´ıdos. El valor K est´a para
que estas variables no crezcan demasiado (s y r siempre est´an entre 0 y K-1).

Explique por que funciona sin usar mutexes ni ninguna primitiva de sincronizaci´on. ¿Puede generalizar a
n procesos?
Funciona porque al haber dos procesos y dos variables que cada uno modifica por separado, no hay una condicion de carrera que no permita
implementar esto sin mutexes o primitiva de sincronizacion.
No se puede generalizar a n procesos porque en cuyo caso de que haya mas de un productor/consumidor, sí habria condicion de carrera
sobre las variables compartidas.
*/

#define B 2
#define K (2 * B)

volatile int s, r, buf[B];

static inline int diff() { return (K + s - r) % K; }

void *prod(void *_arg)
{
    int cur = 0;
    while (1)
    {
        while (diff() == B)
            ;
        buf[s % B] = cur++;
        s = (s + 1) % K;
    }
}

void *cons(void *_arg)
{
    int cur;
    while (1)
    {
        while (diff() == 0)
            ;
        cur = buf[r % B];
        r = (r + 1) % K;
        printf("Le´ı %d\n", cur);
    }
}
