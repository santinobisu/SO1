/*
Ej. 9. Compare la performance del jard´ın ornamental (para una misma cantidad de visitantes totales)
para las siguiente implementaciones. Explique las diferencias (si las hay).

a) sin sincronizaci´on
b) usando el algoritmo de Peterson
c) usando incl
d) usando un pthread mutex t
e) usando un solo molinete sin multithreading.


a- Hay pérdida de casi la mitad de visitantes.
b- Hay pérdida en multiprocesador pero no la hay en uno solo.
c- Hay pérdida en multiprocesador pero no la hay en uno solo.
d- No hay pérdida.
e- No hay pérdida pues no hay más de un hilo.
*/