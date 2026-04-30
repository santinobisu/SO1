#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

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

void wait_for_clients(int lsock, tablaHash *tabla)
{
	int csock;
    pthread_t hilo1;
    
	/* Esperamos una conexión, no nos interesa de donde viene */
	csock = accept(lsock, NULL, NULL);
	if (csock < 0)
    quit("accept");

    args *argumentos = malloc(sizeof(args));

    argumentos->csock = csock;
    argumentos->tabla = tabla;	/* Atendemos al clciente */
    pthread_create(&hilo1, NULL, handle_conn, argumentos);

	/* Volvemos a esperar conexiones */
	wait_for_clients(lsock, tabla);
    pthread_join(&hilo1, NULL);
}

int main (){

    tablaHash *tabla = crear_tablaHash();

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

	/* Esperamos una conexión, no nos interesa de donde viene */
    wait_for_clients(lsock, tabla);

    close(lsock);
    pthread_mutex_destroy(&lock);
    eliminar_tablaHash(tabla);
    return 0;
}