/* Ej. 2. ¿C´uales de los siguientes componentes del estado de un programa se comparten entre los threads
de un proceso?
a) Valores de registros
b) Heap
c) Variables globales
d) Stack

Respuesta:
En un proceso con múltiples threads:

Heap → ✅ Compartido entre todos los threads
Variables globales → ✅ Compartidas también

En cambio:

Valores de registros → ❌ cada thread tiene los suyos
Stack → ❌ cada thread tiene su propia pila

💡 Idea clave:
Todo lo que pertenece al proceso se comparte (heap, globales), y todo lo que pertenece a la ejecución del thread es privado (stack, registros).

Así que la respuesta correcta es: b) Heap y c) Variables globales ✔️:
 */