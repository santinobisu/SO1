#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

void quit(char *s)
{
	perror(s);
	abort();
}

void wait_for_clients(int lsock)
{
	int csock;

	/* Esperamos una conexión, no nos interesa de donde viene */
	csock = accept(lsock, NULL, NULL);
	if (csock < 0)
		quit("accept");

	/* Atendemos al cliente */
	printf("conexion establecida\n");

	/* Volvemos a esperar conexiones */
	wait_for_clients(lsock);
}

int main (){
    struct sockaddr_in sa;
	int lsock;
	int rc;
	int yes = 1;
    lsock = socket(AF_INET, SOCK_STREAM, 0);
    if (lsock < 0)
		quit("socket");

    if (setsockopt(lsock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == 1)
		quit("setsockopt");

    sa.sin_family = AF_INET;
	sa.sin_port = htons(4040);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
    rc = bind(lsock, (struct sockaddr *)&sa, sizeof (sa));
	if (rc < 0)
		quit("bind");

    rc = listen(lsock, 10);
	if (rc < 0)
		quit("listen");

    int csock;

	/* Esperamos una conexión, no nos interesa de donde viene */
    wait_for_clients(lsock);
	/* csock = accept(lsock, NULL, NULL);
	if (csock < 0)
		quit("accept");

    printf("conexion establecida\n"); */
    close(lsock);
    return 0;
}