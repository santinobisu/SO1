#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    int pipefd[2];
    char buff;
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
        }
    
    __pid_t pid1= fork();

    if(pid1== 0){
        close(pipefd[1]); 
        while (read(pipefd[0],&buff, 1)>0)
            write(STDOUT_FILENO, &buff,1);
        write(STDOUT_FILENO,"\n" ,1);
        close(pipefd[0]);        
    }
    else {
        close(pipefd[0]);
        char cadena[100]; 
        scanf("%s",cadena);
        write(pipefd[1], cadena, strlen(cadena));
        close(pipefd[1]);
        wait();
    }
    return 0; 
}