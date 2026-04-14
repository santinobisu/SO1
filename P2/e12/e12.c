#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t a = PTHREAD_MUTEX_INITIALIZER;

void *foo(void *_arg)
{
    pthread_mutex_lock(&a);
    printf("Foo!\n");
    sleep(1);
    pthread_mutex_unlock(&a);
    return NULL;
}

int main()
{
    pthread_t th;
    pthread_create(&th, NULL, foo, NULL);
    pthread_mutex_t b = a;
    pthread_mutex_lock(&b);
    printf("Bar!\n");
    sleep(1);
    pthread_mutex_unlock(&b);
    pthread_join(th, NULL);
    printf("-----\n");
    return 0;
}

/*
El comportamiento del programa es predecible y el esperado; Primero se ejecuta el código del main
que printea "Bar!", previamente lockeando el mutex, luego de 1 segundo lo desbloquea y el thread
creado se ejecuta lockeando el mutex y printeando "Bar!", para luego desbloquearlo y el programa
pase a finalizarse.
*/