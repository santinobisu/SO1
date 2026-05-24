/* 
Ej. 11 (Servidor de turnos). 
El archivo skel server.c implementa un peque˜no servidor que recibe conexiones por un puerto TCP (4040) y responde a cada pedido con un entero ´unico.
Los pedidos, enviados por los clientes, son simplemente una l´ınea NUEVO terminada por \n. 
Para cerrar una conexi´on, el cliente env´ıa CHAU. Como est´a escrito, el servidor s´olo puede atender a un cliente a la vez, dejando en espera a todo el resto hasta que se cierre la conexi´on con el primer cliente. 
Para recibir conexiones TCP, el proceso debe: ˆ ˆ ˆ ˆ Llamar a socket(AF_INET, SOCK_STREAM, 0) para conseguir un socket (un fd) Usar bind para asociarlo a un puerto. Usar listen para permitir que acepte conexiones Llamara accept ahora bloquea hasta que se reciba una conexi´on. accept devuelve un fdrepresentando a la conexi´on, y puede usar read/write sobre el mismo. 
Tareas: a) Modifique el servidor para atender concurrentemente a todas las conexiones abiertas levantando un nuevo proceso por cada conexi´on. Nota: todos los clientes deber´an poder hacer pedidos sin esperar a otros, y siempre debe poder conectarse un nuevo cliente. 
b) Use memoria compartida entre los procesos para mantener el ´ultimo entero enviado a un cliente. ¿Qu´e necesita tener en cuenta para garantizar que dos pedidos nunca reciben el mismo entero? 
*/
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

const char *name = "SO";
const int PAGESIZE = 4096;

void quit(char *s)
{
	perror(s);
	abort();
}

int fd_readline(int fd, char *buf)
{
	int rc;
	int i = 0;

	/*
	 * Leemos de a un caracter (no muy eficiente...) hasta
	 * completar una línea.
	 */
	while ((rc = read(fd, buf + i, 1)) > 0) {
		if (buf[i] == '\n')
			break;
		i++;
	}

	if (rc < 0)
		return rc;

	buf[i] = 0;
	return i;
}

void handle_conn(int csock, int *shared_memory)
{
	char buf[200];
	int rc;

	while (1) {
		/* Atendemos pedidos, uno por linea */
		rc = fd_readline(csock, buf);
		if (rc < 0)
			quit("read... raro");

		if (rc == 0) {
			/* linea vacia, se cerró la conexión */
			close(csock);
			return;
		}

		if (!strcmp(buf, "NUEVO")) {
			char reply[20];
			sprintf(reply, "%d\n", (*shared_memory)++);
			write(csock, reply, strlen(reply));
		} else if (!strcmp(buf, "CHAU")) {
			close(csock);
			return;
		}
	}
}

void wait_for_clients(int lsock, int *shared_memory)
{
	int csock;

	/* Esperamos una conexión, no nos interesa de donde viene */
	csock = accept(lsock, NULL, NULL);
	if (csock < 0)
		quit("accept");

    __pid_t pid = fork();
    if(pid== 0){
        
        /* Atendemos al cliente */
	    handle_conn(csock,shared_memory);
    
	    /* Volvemos a esperar conexiones */
    }
    else {
        wait_for_clients(lsock,shared_memory);
        wait();
    }
	
}
int *create_shm_memory(){
    int fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, PAGESIZE);
    int *memoria = mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    return memoria;
}

/* Crea un socket de escucha en puerto 4040 TCP */
int mk_lsock()
{
	struct sockaddr_in sa;
	int lsock;
	int rc;
	int yes = 1;

    
	/* Crear socket */
	lsock = socket(AF_INET, SOCK_STREAM, 0);
	if (lsock < 0)
		quit("socket");

	/* Setear opción reuseaddr... normalmente no es necesario */
	if (setsockopt(lsock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == 1)
		quit("setsockopt");

	sa.sin_family = AF_INET;
	sa.sin_port = htons(4040);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	/* Bindear al puerto 4040 TCP, en todas las direcciones disponibles */
	rc = bind(lsock, (struct sockaddr *)&sa, sizeof sa);
	if (rc < 0)
		quit("bind");

	/* Setear en modo escucha */
	rc = listen(lsock, 10);
	if (rc < 0)
		quit("listen");

	return lsock;
}

int main()
{
	int lsock;
	lsock = mk_lsock();
    int *shared_memory = create_shm_memory();
    *shared_memory = 0;
	wait_for_clients(lsock, shared_memory);
    shm_unlink(name);
}