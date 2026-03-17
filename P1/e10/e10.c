#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

/* Ej. 10 (Signal Pong). Hacer un programa que tenga el siguiente comportamiento:
Luego del fork, el padre env´ıa una se˜nal SIGUSR1 al hijo y entra en un loop infinito. El hijo espera en
un loop infinito. Ambos (padre e hijo) cuando reciven una se˜nal SIGUSR1 responden lo mismo. (el padre
al hijo y el hijo al padre). ¿Qu´e pasa si cambiamos los loops infinitos por pause()?. Usar las funciones
signal() y sigaction(). */

__pid_t pid;

void handler(int sig){
    printf("%d: recibi la senal\n",getpid());
    kill(pid, SIGUSR1);
};

int main(){
    int pipefd[2];
    
    if(pipe(pipefd) == -1) {
        perror("todo mal\n");
        exit(EXIT_FAILURE);
    }
    __pid_t child = fork();
    if(child == 0){
        close(pipefd[1]);
        read(pipefd[0],&pid,sizeof(pid));
        signal(SIGUSR1,handler);
        while(1) pause();

    }
    else {
        close(pipefd[0]);
        __pid_t pid_p= getpid();
        pid= child;
        write(pipefd[1],&pid_p,sizeof(pid_p));
        signal(SIGUSR1,handler);
        kill(pid,SIGUSR1);
        while(1) pause();
    }
}