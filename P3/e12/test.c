#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "e12.h"

// Declaramos el semáforo globalmente
Semaforo semaforo;

// Función que ejecutarán nuestros hilos (simulan ser procesos/tareas)
void* rutina_hilo(void* arg) {
    int id = *(int*)arg;

    printf("🟡 [Hilo %d] Llegó y hace WAIT. Esperando luz verde...\n", id);

    // 🔴 Operación WAIT: Si el semáforo está en 0, se bloquea. Si es > 0, resta 1 y pasa.
    semaforo_wait(&semaforo);
    
    printf("🟢 [Hilo %d] ¡Entró a la sección crítica! (Semáforo cerrado para los demás)\n", id);

    // Simulamos que el hilo está haciendo un trabajo importante que toma tiempo
    printf("   [Hilo %d] ⚙️  Trabajando en la sección crítica...\n", id);
    sleep(2); // Pausa de 2 segundos para que veas el efecto

    printf("🔴 [Hilo %d] Terminó el trabajo. Hace POST y sale...\n", id);
    
    // 🟢 Operación POST: Suma 1 al semáforo, liberando a otro hilo que esté en WAIT.
    semaforo_post(&semaforo);

    printf("⚪ [Hilo %d] Ejecución finalizada.\n\n", id);
    
    free(arg); // Liberamos la memoria del ID
    return NULL;
}

int main() {
    int cantidad_hilos = 3; // Vamos a lanzar 3 hilos para probar
    pthread_t hilos[cantidad_hilos];

    printf("==================================================\n");
    printf("🚦 INICIANDO TEST DE SEMÁFOROS (INIT, WAIT, POST) 🚦\n");
    printf("==================================================\n\n");

    // 🟢 Operación INIT: Inicializamos el semáforo.
    // Argumentos: puntero al semáforo, 0 (para usar entre hilos del mismo proceso), 1 (valor inicial)
    if (semaforo_init(&semaforo,  1) != 0) {
        perror("❌ Error al inicializar el semáforo");
        return 1;
    }
    printf("✅ Semáforo inicializado correctamente (Valor inicial: 1)\n");
    printf("--------------------------------------------------\n\n");

    // Creamos los hilos
    for (int i = 0; i < cantidad_hilos; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        if (pthread_create(&hilos[i], NULL, &rutina_hilo, id) != 0) {
            perror("❌ Error al crear el hilo");
            return 1;
        }
        // Pequeña pausa para que no lleguen todos exactamente al mismo microsegundo 
        // y el output se lea más ordenado.
        usleep(100000); 
    }

    // Esperamos a que todos los hilos terminen su trabajo
    for (int i = 0; i < cantidad_hilos; i++) {
        pthread_join(hilos[i], NULL);
    }

    // Destruimos el semáforo para liberar recursos

    printf("==================================================\n");
    printf("🏁 TEST FINALIZADO CON ÉXITO 🏁\n");
    printf("==================================================\n");

    return 0;
}