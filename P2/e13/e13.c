/* 
Ej. 13 (dif´ıcil). Para el algoritmo de Peterson, ¿puede ubicarse el mfence entre la asignaci´on al flag y
la asignaci´on a turn? Justifique.

No, porque el dato de la asignación a turn puede quedarse en el store buffer de uno de los 
procesadores, haciendo que el otro no registre el cambio de turno.
*/