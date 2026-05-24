#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

/* Ej. 2. Conteste las siguientes preguntas. Puede ser ´util usar strace para monitorear las llamadas a
sistema de un proceso, o top para ver la tabla de procesos del sistema.

a) Si se cierra el file descriptor de la salida est´andar (1) ¿qu´e pasa al escribir al mismo?

b) Si se cierra el file descriptor de la entrada est´andar (0) ¿qu´e pasa al intentar leer del mismo?

c) Si un file descriptor se duplica con dup() ¿qu´e pasa al cerrar una de las copias?

d) Al hacer fork(), ¿c´omo cambia el valor de getpid()? ¿Y al hacer exec()?

e) Con fork(), cree dos procesos y haga que el hijo termine (con exit() o retornando del main) y que
el padre duerma indefinidamente sin hacer wait(). ¿C´omo aparece el hijo en la tabla de procesos?
¿Por qu´e sigue existiendo?

f) Al hacer un malloc de 1GB ¿aumenta el uso de memoria de un proceso? Explique.

g) ¿Qu´e pasa con el uso de memoria de un proceso al realizar fork()? ¿Y exec()?

h) ¿Qu´e pasa con los file descriptor de un proceso al hacer fork()? ¿Y exec()?

i) El comando de cambio de directorio cd suele ser un built-in de la shell. ¿Puede implementarse
mediante un programa al igual que, ej., ls? */

// a) No escribe nada
/* int main()
{
    close(1);
    printf("Chincho\n");
    return 0;
} */

// b) No se puede leer de la entrada estandar 
/* int main()
{
    char buff[100];
    close(0);
    fgets(buff,20,stdin);
    return 0;
} */

// c) Sigue funcionando
/* int main()
{
    int fd = dup(1);
    close(fd);
    printf("bichu\n");
    return 0;
} */

// d) Al hace fork(), el valor getpid() aumenta en uno en el llamado del hijo. Al hacer exec(), el segundo llamado de getpid() no se ejecuta
/* int main()
{
    __pid_t pid = getpid();
    printf("pid 1: %d\n", pid);
    execl("/bin/ls", "messi", NULL);
    pid = getpid();
    printf("pid 2: %d\n", pid);
    return 0;
} */

// e) El hijo aparece como un proceso zombie. Sigue existiendo porque el padre al no poder recibir la notificacion del fallecimiento del hijo, dicho hijo queda en zombie
/* int main()
{    __pid_t child = fork();
    if (child == 0) exit(0);
    else while (1);

    return 0;
} */

// f) No, no aumenta. Esto es porque el sistema operativo no le da memoria fisica al proceso, sino memoria virtual y no esencialmente todo lo que pide

/* int main()
{
    int i= 0;
    int* asd = malloc(sizeof(int)*(2 << 25));
    while(1) {
        usleep(1¿¿00);
        asd[i]= i;
        i++;
        }
    return 0;
} */

// g) Al usar fork() el uso de memoria se duplica pues los procesos no comparten el mismo espacio de memoria. Al utilizar exec() no aumenta pues no se crea otro proceso
/* int main()
{
    execl("/bin/ls", "hola", NULL);
    while(1);
    return 0;
} */

// h) Al hacer fork(), el proceso hijo hereda una copia de todos los descriptores de archivo (FD) del padre, apuntando a los mismos archivos. Al ejecutar exec(), los FDs permanecen abiertos.
/* int main()
{
    int fd = open("fdtest.txt", O_RDWR, "rw");
    __pid_t child = fork();
    if (child == 0) printf("Child fd: %d\n", fd);
    else printf("Parent fd: %d\n", fd);
    return 0;
} */

// i) No, cd no puede implementarse como un programa externo ejecutable (como ls o pwd) y funcionar correctamente. Debe ser un built-in (interno) de la shell porque un programa externo crea su propio proceso hijo, y al cambiar de directorio, el cambio se pierde cuando el programa termina, sin afectar a la shell principal.

