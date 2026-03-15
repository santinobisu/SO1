#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

/* Ej. 3. El comando yes imprime l´ıneas conteniendo una ‘y’ infinitamente. Es usado para simular una
respuesta afirmativa para instaladores o programas similares (i.e. “s´ı a todo”) haciendo simplemente
yes | ./installer. ¿C´omo piensa que est´a implementado? Al ejecutar un pipeline como el anterior
¿c´omo es el uso de CPU del proceso yes? */

// Arma un buffer grande con muchas "y\n", y lo escribe repetidamente con write(). Al ejecutar yes | ./installer se crea un pipeline de yes (stdout) a installer (stdin). 
// En un pipeline: si el consumidor lee rápido: alto uso de CPU. Si el pipe se llena: write() bloquea y el CPU usado es muy bajo.