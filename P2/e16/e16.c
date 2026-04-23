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
typedef struct _tablaHash
{
    char **arr;
    int tam;
} tablaHash;

typedef struct _args
{
    int csock;
    tablaHash *tabla;
} args;

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

void handle_conn(args *argumentos)
{
	char buf[200];
	int rc;

	while (1) {
		/* Atendemos pedidos, uno por linea */
		rc = fd_readline(argumentos->csock, buf);
		if (rc < 0)
        {
            free(argumentos);
            quit("read... raro");
        }
			

		if (rc == 0) {
			/* linea vacia, se cerró la conexión */
			close(argumentos->csock);
            free(argumentos);
			return;
		}

        //strtok y demas
        pthread_mutex_lock(&lock);
        char *mensaje= strtok(buf, " ");
        char *comando = strcpy(comando, mensaje);
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
                    insertar_v_tablaHash(argumentos->tabla, key, mensaje);
                    printf("OK\n");
                }
                
            }
        }
        else if (strcmp(comando, "DEL") == 0)
        {
            mensaje = strtok(NULL, " ");
            if (mensaje == NULL) printf("EINVAL\n");
            else
            {
                eliminar_v_tablaHash(argumentos->tabla, mensaje);
                printf("OK\n");
            }  
        }
        else if (strcmp(comando, "GET") == 0)
        {
            mensaje = strtok(NULL, " ");
            if (mensaje == NULL) printf("EINVAL\n");
            else
            {
               char* c = buscar_v_tablaHash(argumentos->tabla, mensaje);
               if (c[0] == '\n') printf("NOTFOUND\n");
               else 
               { 
                   printf("%s\n", c);
                   printf("OK\n");
               }
            }
            
        }
        else
        {
            printf("EINVAL\n");
        }
        pthread_mutex_unlock(&lock);
	}
}

void wait_for_clients(int lsock, tablaHash *tabla, int epollfd, struct epoll_event *events, struct epoll_event *ev)
{
	int csock;
    int nfds;

    for (;;) {
    nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
    if (nfds == -1) {
        perror("epoll_wait");
        exit(EXIT_FAILURE);
    }

    for (int n = 0; n < nfds; ++n) {
        if (events[n].data.fd == lsock) {
            csock = accept(lsock, NULL, NULL);
            if (csock == -1) {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            setnonblocking(csock);
            ev->events = EPOLLIN | EPOLLET;
            ev->data.fd = csock;
            if (epoll_ctl(epollfd, EPOLL_CTL_ADD, csock,ev) == -1) {
                perror("epoll_ctl: csock");
                exit(EXIT_FAILURE);
            }
        } else {
            do_use_fd(events[n].data.fd);
        }
    }
}
    
	/* Esperamos una conexión, no nos interesa de donde viene */

    args *argumentos = malloc(sizeof(args));

    argumentos->csock = csock;
    argumentos->tabla = tabla;	/* Atendemos al clciente */
    pthread_create(&hilo1, NULL, handle_conn, argumentos);

	/* Volvemos a esperar conexiones */
    pthread_join(&hilo1, NULL);
}

int crear_epoll(struct epoll_event *ev, int lsock)
{
    epollfd = epoll_create1(0);
    if (epollfd == -1) {
    perror("epoll_create1");
    exit(EXIT_FAILURE);
    }

    ev->events = EPOLLIN;
    ev->data.fd = lsock;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, lsock, ev) == -1) {
    perror("epoll_ctl: lsock");
    exit(EXIT_FAILURE);
    }

    return epollfd;
}

int main (){
    
    tablaHash *tabla = crear_tablaHash();
    struct epoll_event ev, events[MAX_EVENTS];    
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

    int epollfd = crear_epoll(&ev, lsock);

	/* Esperamos una conexión, no nos interesa de donde viene */
    wait_for_clients(lsock, tabla, epollfd, events, &ev);

    close(lsock);
    pthread_mutex_destroy(&lock);
    eliminar_tablaHash(tabla);

    return 0;
}

struct epoll_event ev, events[MAX_EVENTS];
int lsock, conn_sock, nfds, epollfd;

 Code to set up listening socket, 'lsock',
    (socket(), bind(), listen()) omitted. 

epollfd = epoll_create1(0);
if (epollfd == -1) {
    perror("epoll_create1");
    exit(EXIT_FAILURE);
}

ev.events = EPOLLIN;
ev.data.fd = lsock;
if (epoll_ctl(epollfd, EPOLL_CTL_ADD, lsock, &ev) == -1) {
    perror("epoll_ctl: lsock");
    exit(EXIT_FAILURE);
}

for (;;) {
    nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
    if (nfds == -1) {
        perror("epoll_wait");
        exit(EXIT_FAILURE);
    }

    for (n = 0; n < nfds; ++n) {
        if (events[n].data.fd == lsock) {
            conn_sock = accept(lsock,
                                (struct sockaddr *) &addr, &addrlen);
            if (conn_sock == -1) {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            setnonblocking(conn_sock);
            ev.events = EPOLLIN | EPOLLET;
            ev.data.fd = conn_sock;
            if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock,
                        &ev) == -1) {
                perror("epoll_ctl: conn_sock");
                exit(EXIT_FAILURE);
            }
        } else {
            do_use_fd(events[n].data.fd);
        }
    }
}