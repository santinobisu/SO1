/*
Ej. 3 (Fil´osofos Comensales, Dijkstra). Cinco fil´osofos se sientan alrededor
de una mesa redonda y viven la buena vida alternando entre comer y pensar.
Cada fil´osofo tiene su plato de pasta, un tipo particular de fideos que requieren
dos tenedores para ser comidos. Entre cada par de fil´osofos consecutivos hay un
tenedor, y cada fil´osofo s´olo puede alcanzar los que est´an a su izquierda y derecha.
Para comer, los fil´osofos acordaron primero tomar el tenedor a su derecha y luego el de a su izquierda.
Si cualquier fil´osofo toma un tenedor y el otro est´a ocupado,
se quedar´a esperando, con un tenedor en la mano, hasta que pueda tomar el otro
tenedor, para luego empezar a comer. Una vez que un fil´osofo terminar de comer
deja los tenedores sobre la mesa y piensa hasta que le vuelve a dar hambre, cuando
repite el procedimiento.
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N_FILOSOFOS 5
#define ESPERA 5000000

pthread_mutex_t tenedor[N_FILOSOFOS];
pthread_cond_t cond[N_FILOSOFOS];
int turno[N_FILOSOFOS] = {1};
pthread_mutex_t *izq(int i) { return &tenedor[i]; }
pthread_mutex_t *der(int i) { return &tenedor[(i + 1) % N_FILOSOFOS]; }

void pensar(int i)
{
    printf("Filosofo %d pensando...\n", i);
    usleep(random() % ESPERA);
}

void comer(int i)
{
    printf("Filosofo %d comiendo...\n", i);
    usleep(random() % ESPERA);
}

void tomar_tenedores(int i)
{
    pthread_mutex_lock(der(i));
    while(!(turno[i])) pthread_cond_wait(&cond[i], der(i));
    turno[(i + 1) % N_FILOSOFOS] = 0;
    pthread_mutex_lock(izq(i));
}

void dejar_tenedores(int i)
{
    pthread_mutex_unlock(der(i));
    pthread_cond_signal(&cond[(i + 1) % N_FILOSOFOS]);
    turno[(i + 1) % N_FILOSOFOS] = 1;
    pthread_mutex_unlock(izq(i));
}

void *filosofo(void *arg)
{
    int i = arg - (void *)0;
    while (1)
    {
        tomar_tenedores(i);
        comer(i);
        dejar_tenedores(i);
        pensar(i);
    }
}

int main()
{
    pthread_t filo[N_FILOSOFOS];
    int i;
    for (i = 0; i < N_FILOSOFOS; i++)
    {
        pthread_mutex_init(&tenedor[i], NULL);
        pthread_cond_init(&cond[i], NULL);
    }
    for (i = 0; i < N_FILOSOFOS; i++)
        pthread_create(&filo[i], NULL, filosofo, i + (void *)0);
    for (i = 0; i < N_FILOSOFOS; i++)
        pthread_join(filo[i], NULL);
    return 0;
}

/*
a- Puede terminar en deadlock si los 5 filósofos deciden comer al mismo tiempo, en donde cada uno tomará el tenedor de su 
derecha y se queda esperando el tenedor de la izquierda, que ya estará tomado por el filósofo de su izquierda.

b- Esto funciona porque se rompe el circular wait que hay entre los recursos (tenedores) de la mesa, imposiblitando el caso
de que cada filósofo tome el tenedor a su derecha al mismo tiempo y se produzca un deadlock.

void pensar(int i)
{
    printf("Filosofo %d pensando...\n", i);
    usleep(random() % ESPERA);
}

void comer(int i)
{
    printf("Filosofo %d comiendo...\n", i);
    usleep(random() % ESPERA);
}

void tomar_tenedores(int i)
{
    pthread_mutex_lock(der(i));
    pthread_mutex_lock(izq(i));
}

void tomar_tenedores_izq(int i)
{
    pthread_mutex_lock(izq(i));
    pthread_mutex_lock(der(i));
}

void dejar_tenedores(int i)
{
    pthread_mutex_unlock(der(i));
    pthread_mutex_unlock(izq(i));
}

void *filosofo(void *arg)
{
    int i = arg - (void *)0;
    while (1)
    {
        tomar_tenedores(i);
        comer(i);
        dejar_tenedores(i);
        pensar(i);
    }
}

void *filosofo_zurdo(void *arg)
{
    int i = arg - (void *)0;
    while (1)
    {
        tomar_tenedores_izq(i);
        comer(i);
        dejar_tenedores(i);
        pensar(i);
    }
}

int main()
{
    pthread_t filo[N_FILOSOFOS];
    int i;
    for (i = 0; i < N_FILOSOFOS; i++)
        pthread_mutex_init(&tenedor[i], NULL);
    for (i = 0; i < N_FILOSOFOS - 1; i++)
        pthread_create(&filo[i], NULL, filosofo, i + (void *)0);
    pthread_create(&filo[N_FILOSOFOS - 1], NULL, filosofo_zurdo, (N_FILOSOFOS - 1) +  (void *)0);
    for (i = 0; i < N_FILOSOFOS; i++)
        pthread_join(filo[i], NULL);
    return 0;
}

c- Funciona ya que en el caso que n-1 hilos tomen el de su derecha hay un tenedor que queda libre el cual no puede ser tomado 
por la derecha pero si por la izquierda asi permitiendo su uso y posteriormente liberarlo, asi evitando el deadlock.

sem_t semaforo;
pthread_mutex_t tenedor[N_FILOSOFOS];
pthread_mutex_t *izq(int i) { return &tenedor[i]; }
pthread_mutex_t *der(int i) { return &tenedor[(i + 1) % N_FILOSOFOS]; }

void pensar(int i)
{
    printf("Filosofo %d pensando...\n", i);
    usleep(random() % ESPERA);
}

void comer(int i)
{
    printf("Filosofo %d comiendo...\n", i);
    usleep(random() % ESPERA);
}

void tomar_tenedores(int i)
{
    pthread_mutex_lock(der(i));
    pthread_mutex_lock(izq(i));
}

void dejar_tenedores(int i)
{
    pthread_mutex_unlock(der(i));
    pthread_mutex_unlock(izq(i));
}

void *filosofo(void *arg)
{
    int i = arg - (void *)0;
    while (1)
    {
        sem_wait(&semaforo);
        tomar_tenedores(i);
        comer(i);
        dejar_tenedores(i);
        sem_post(&semaforo);
        pensar(i);
    }
}

int main()
{
    if (sem_init(&semaforo, 0, N_FILOSOFOS - 1) == -1) perror("semaforo no creado");
    pthread_t filo[N_FILOSOFOS];
    int i;
    for (i = 0; i < N_FILOSOFOS; i++)
        pthread_mutex_init(&tenedor[i], NULL);
    for (i = 0; i < N_FILOSOFOS; i++)
        pthread_create(&filo[i], NULL, filosofo, i + (void *)0);
    for (i = 0; i < N_FILOSOFOS; i++)
        pthread_join(filo[i], NULL);
    return 0;
}

d- Sí, funciona. Es eficiente? 


*/