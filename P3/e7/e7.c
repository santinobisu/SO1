/*

Ej. 7 (Problema de los Fumadores, Patil). Tres procesos tratan de fumar cada vez que pueden.
Para hacerlo necesitan tres ingredientes: tabaco, papel y f´osforos. Cada uno tiene una cantidad ilimitada
de uno de estos ingredientes. Esto es, un fumador tiene tabaco, otro tiene papel y el ´ultimo tiene f´osforos.
Los fumadores no se prestan los ingredientes entre ellos, pero hay un cuarto proceso, el agente, con
cantidad ilimitada de todos los ingredientes, que repetidamente pone a disposici´on de los fumadores dos
de los tres ingredientes, eligi´endolos al azar. Cada vez que esto pasa, el fumador que tiene el ingrediente
restante deber´ıa proceder a hacerse un cigarrillo y fumar. Al terminar de fumar (y no antes) el fumador
avisa al agente que termin´o, para que el mismo pueda seguir atendiendo.

El problema consiste en garantizar que el sistema progresa normalmente, es decir que los fumadores toman
los ingredientes cuando les corresponde, fuman, y luego notifican al agente que se puede continuar. Los
fumadores deben sincronizar entre ellos; no debe modificarse al agente en lo m´as min´ımo. S´ı pueden
crearse nuevos sem´aforos, mutexes, y threads.
a) ¿C´omo puede ocurrir un deadlock en esta implementaci´on ingenua?
b) ¿Pueden ordenarse los recursos para evitar el problema?
c) Implemente una soluci´on (recuerde: sin modificar al agente) y expl´ıquela.
d) Piense soluciones alternativas si fuera aceptable modificar al agente.
e) Extra: ¿c´omo se relaciona este ejercicio al servidor de turnos de la pr´actica anterior?

a- Puede ocurrir un deadlock si un fumador toma uno de los dos recursos que necesita, luego otro proceso toma el otro recurso que 
necesitaba el proceso, produciendose asi un hold and wait, en donde ninguno de los dos va a avanzar.

b- No, no se puede.

c- La solucion es que cuando un proceso toma un recurso, se fije primero si esta el otro, si no esta entonces suelta el recurso
que tomó y se saltea la iteracion, volviendo a su estado inicial.

d- En dicho caso, la solucion seria darle el turno al fumador directamente respecto al caso que tocó.
*/



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t tabaco, papel, fosforos, otra_vez;
pthread_mutex_t lock_fumadores = PTHREAD_MUTEX_INITIALIZER;

void agente()
{
    while (1)
    {
        sem_wait(&otra_vez);
        int caso = random() % 3;
        if (caso != 0)
            sem_post(&fosforos);
        if (caso != 1)
            sem_post(&papel);
        if (caso != 2)
            sem_post(&tabaco);
    }
}

void fumar(int fumador)
{
    printf("Fumador %d: Puf! Puf! Puf!\n", fumador);
    sleep(1);
}

void *fumador1(void *arg)
{   
    int hay_papel = 0;
    while (1)
    {   
        sem_getvalue(&papel, &hay_papel);
        sem_wait(&tabaco);
        if(!hay_papel)
        {
            sem_post(&tabaco);
            continue;
        }
        sem_wait(&papel);
        fumar(1);
        
        sem_post(&otra_vez);
    }
}

void *fumador2(void *arg)
{
    int hay_tabaco = 0;
    while (1)
    {  
        sem_getvalue(&tabaco, &hay_tabaco);
        sem_wait(&fosforos);
        if(!hay_tabaco)
        {
            sem_post(&fosforos);
            continue;
        }
        sem_wait(&tabaco);
        fumar(2);
        sem_post(&otra_vez);
    }
}

void *fumador3(void *arg)
{
    int hay_fosforos = 0;
    while (1)
    {   
        sem_getvalue(&fosforos, &hay_fosforos);
        sem_wait(&papel);
        if(!hay_fosforos)
        {
            sem_post(&papel);
            continue;
        }
        sem_wait(&fosforos);
        fumar(3);
        sem_post(&otra_vez);
    }
}

int main()
{
    pthread_t s1, s2, s3;
    sem_init(&tabaco, 0, 0);
    sem_init(&papel, 0, 0);
    sem_init(&fosforos, 0, 0);
    sem_init(&otra_vez, 0, 1);
    pthread_create(&s1, NULL, fumador1, NULL);
    pthread_create(&s2, NULL, fumador2, NULL);
    pthread_create(&s3, NULL, fumador3, NULL);
    agente();
    return 0;
}
