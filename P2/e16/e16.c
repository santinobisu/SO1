#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/epoll.h>

#define MAX_EVENTS 4


struct epoll_event ev, events[MAX_EVENTS];
int epollfd;

typedef struct _tablaHash
{
    char **arr;
    int tam;
} tablaHash;

tablaHash *tabla;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

tablaHash *crear_tablaHash()
{
    tablaHash *t = malloc(sizeof(tablaHash));
    t->tam = 11;
    t->arr = malloc(sizeof(char*) * t->tam);
    for (int i = 0; i < t->tam; i++)
    {
        t->arr[i] = malloc(sizeof(char)*1);
        t->arr[i][0] = '\n';
    }
        
    return t;
}

void eliminar_tablaHash(tablaHash *t)
{
    for (int i = 0; i < t->tam; i++)
        free(t->arr[i]);
    free(t);
}

int hashearValor(char *k, tablaHash *t)
{
    return (int) k[0] % t->tam;
}

void insertar_v_tablaHash(tablaHash *t, char *k, char *v)
{
    unsigned idx = hashearValor(k, t);
    free(t->arr[idx]);
    t->arr[idx] = strdup(v);
}

void eliminar_v_tablaHash(tablaHash *t, char *k)
{
    unsigned idx = hashearValor(k,t);
    free(t->arr[idx]);
    t->arr[idx] = strdup("\n");
}

char *buscar_v_tablaHash(tablaHash *t, char *k)
{   
    unsigned idx = hashearValor(k,t);
    return t->arr[idx];
}


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

void handle_conn(int csock)
{
	char buf[200];
	int rc;

	while (1) {
		/* Atendemos pedidos, uno por linea */

		rc = fd_readline(csock, buf);


		if (rc < 0) quit("read... raro");

		if (rc == 0) {
			/* linea vacia, se cerró la conexión */
			close(csock);
			return;
		}

        //strtok y demas
        pthread_mutex_lock(&lock);

        char *mensaje= strtok(buf, " ");

        char *comando;
        comando = strdup(mensaje);

        if (strcmp(comando, "PUT") == 0)
        {

            char *key = strtok(NULL, " ");

            if (key == NULL) printf("EINVAL\n");
            else
            {

                mensaje = strtok(NULL, " " );

                if(mensaje == NULL) printf ("EINVAL\n");
                else 
                {
                    insertar_v_tablaHash(tabla, key, mensaje);
                    printf("OK\n");
                }
                free(comando);
            }
        }
        else if (strcmp(comando, "DEL") == 0)
        {
            mensaje = strtok(NULL, " ");
            if (mensaje == NULL) printf("EINVAL\n");
            else
            {
                eliminar_v_tablaHash(tabla, mensaje);
                printf("OK\n");
            } 
            free(comando);
        }
        else if (strcmp(comando, "GET") == 0)
        {
            mensaje = strtok(NULL, " ");
            if (mensaje == NULL) printf("EINVAL\n");
            else
            {
               char* c = buscar_v_tablaHash(tabla, mensaje);
               if (c[0] == '\n') printf("NOTFOUND\n");
               else 
               { 
                   printf("%s\n", c);
                   printf("OK\n");
               }
            }
            free(comando);
        }
        else
        {
            printf("EINVAL\n");
            free(comando);
        }
        pthread_mutex_unlock(&lock);
	}
}

void wait_for_clients(void *lsockarg)
{
	int csock, nfds;
    int *lsock = lsockarg;

    for (;;) {
                nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
                if (nfds == -1) quit("epoll_wait");

                for (int n = 0; n < nfds; ++n) {
                    if (events[n].data.fd == *lsock) {
                        csock = accept(*lsock, NULL, NULL);
                        if (csock == -1) quit("accept");
                        ev.events = EPOLLIN | EPOLLONESHOT;
                        ev.data.fd = csock;
                        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, csock, &ev) == -1) quit("epoll_ctl: csock");
                        ev.data.fd = *lsock;
                        if (epoll_ctl(epollfd, EPOLL_CTL_MOD, *lsock, &ev) == -1) quit("epoll_ctl: lsock");
                    } else handle_conn(events[n].data.fd);
                }
    }
}

int main (){

    pthread_t h1, h2, h3, h4;
    tabla = crear_tablaHash();

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
	sa.sin_port = htons(3942);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
    rc = bind(lsock, (struct sockaddr *)&sa, sizeof (sa));
	if (rc < 0)
		quit("bind");

    rc = listen(lsock, 10);
	if (rc < 0)
		quit("listen");

    int csock;
    epollfd = epoll_create1(0);
    if (epollfd == -1) quit("epoll_create1");
    ev.events = EPOLLIN;
    ev.data.fd = lsock;

    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, lsock, &ev) == -1) quit("epoll_ctl: listen_sock");

    // crear 4 hilos y pasarle wait for clients
    pthread_create(&h1, NULL, wait_for_clients, &lsock);
    pthread_create(&h2, NULL, wait_for_clients, &lsock);
    pthread_create(&h3, NULL, wait_for_clients, &lsock);
    pthread_create(&h4, NULL, wait_for_clients, &lsock);

    pthread_join(h1, NULL);
    pthread_join(h2, NULL);
    pthread_join(h3, NULL);
    pthread_join(h4, NULL);

	/* Esperamos una conexión, no nos interesa de donde viene */

    close(lsock);

    eliminar_tablaHash(tabla);
    return 0;
}