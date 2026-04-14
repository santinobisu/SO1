/* Para la version ingenua (sin exclusi´on m´utua) del jard´ın ornamental, ¿qu´e pasa cuando compilamos
con optimizaciones? Pista: ver el assembler generado.
Lo que pasa al compilar con optimizaciones es que el compilador puede mantener variables en registros,
reordenar instrucciones, eliminar accesos redundantes a memoria, lo que significa que otros
hilos podrian no ver las actualizaciones intermedias. Esto puede generar resultados incorrectos o
mostrar comportamientos indefinidos.
*/