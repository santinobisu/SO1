#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "e13.h"

// Declaramos el Mutex y la Variable de Condición globalmente
pthread_mutex_t candado;
Vcondicion condicion;

// Variable compartida que el trabajador necesita que cambie
int datos_listos = 0;

// Función del hilo Trabajador (el que hace WAIT)
void* rutina_trabajador(void* arg) {
    printf("👷 [Trabajador] Llegó a la fábrica. Tomando el candado (Mutex)...\n");
    pthread_mutex_lock(&candado);

    printf("👷 [Trabajador] Chequeando si los datos están listos...\n");
    
    // IMPORTANTE: Siempre se usa un 'while' para chequear la condición, no un 'if'
    while (datos_listos == 0) {
        printf("⏳ [Trabajador] Faltan datos. Hace WAIT en la variable de condición...\n");
        printf("   (El WAIT suelta el candado automáticamente y pone a dormir al hilo)\n");
        
        // 🔴 Operación WAIT: Duerme al hilo y libera el Mutex para que otro lo use.
        // Cuando alguien le haga 'signal', se despierta y vuelve a tomar el Mutex automáticamente.
        vcondicion_wait(&condicion, &candado);
        
        printf("🔔 [Trabajador] ¡Se despertó por un SIGNAL! Volvió a tomar el candado.\n");
    }

    printf("✅ [Trabajador] ¡Los datos están en %d! Procesando...\n", datos_listos);
    sleep(1); // Simulamos trabajo
    printf("👷 [Trabajador] Trabajo terminado. Soltando el candado y yéndose a casa.\n\n");
    
    pthread_mutex_unlock(&candado);
    return NULL;
}

// Función del hilo Jefe (el que hace SIGNAL)
void* rutina_jefe(void* arg) {
    printf("👔 [Jefe] Preparando los datos... (esto tomará 3 segundos)\n");
    sleep(3); // Simulamos que tarda en preparar la información

    printf("👔 [Jefe] Datos preparados. Tomando el candado para actualizarlos...\n");
    pthread_mutex_lock(&candado);
    
    datos_listos = 1; // Cambiamos el estado de la variable compartida
    
    printf("📣 [Jefe] Hace SIGNAL en la variable de condición para despertar al trabajador...\n");
    // 🟢 Operación SIGNAL: Despierta a UN hilo que esté esperando en el wait.
    // Si quisieras despertar a TODOS los hilos esperando, usarías pthread_cond_broadcast()
    vcondicion_signal(&condicion);
    
    pthread_mutex_unlock(&candado);
    printf("👔 [Jefe] Candado liberado. Se va a tomar un café.\n\n");

    return NULL;
}

int main() {
    pthread_t hilo_trabajador, hilo_jefe;

    printf("========================================================\n");
    printf("🔔 TEST DE VARIABLES DE CONDICIÓN (INIT, WAIT, SIGNAL) 🔔\n");
    printf("========================================================\n\n");

    // 🟢 Operación INIT: Inicializamos el Mutex y la Variable de Condición
    pthread_mutex_init(&candado, NULL);
    vcondicion_init(&condicion);
    printf("✅ Mutex y Variable de Condición inicializados.\n");
    printf("--------------------------------------------------------\n\n");

    // Creamos los hilos. Lanzamos primero al trabajador para asegurarnos de que se quede esperando.
    pthread_create(&hilo_trabajador, NULL, rutina_trabajador, NULL);
    usleep(500000); // Pausa de medio segundo
    pthread_create(&hilo_jefe, NULL, rutina_jefe, NULL);

    // Esperamos a que ambos terminen
    pthread_join(hilo_trabajador, NULL);
    pthread_join(hilo_jefe, NULL);

    // Destruimos las estructuras para limpiar la memoria


    printf("========================================================\n");
    printf("🏁 TEST FINALIZADO CON ÉXITO 🏁\n");
    printf("========================================================\n");

    return 0;
}