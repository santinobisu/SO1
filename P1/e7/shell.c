#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

// Ejercicio: Hacer una Shell

int main()
{
    
    while(1)
    {   
        printf("New command:\n");
        int largo = 255;
        char buff[largo];
        fgets(buff, largo, stdin);
        buff[strcspn(buff,"\n")] = 0;
        char *token = strtok(buff, " "); //token = comando/path
        //if(strcmp(token, "exit()"))break;
        char inicio[largo];
        strcpy(inicio, "/bin/");
        strcat(inicio, token);
        char* args[largo];
        int redireccion = 0;
        int prox_redireccion;
        int i = 0;
        args[i]= token;
        i++;
        token = strtok(NULL, " ");  
        while (token != NULL)
        {
            if(strcmp(">", token)== 0)
            {
                redireccion = 1;
                prox_redireccion = i+1;
            }
            args[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        args[i]= NULL;
        __pid_t pid= fork();
        if(pid== 0){
            if(redireccion){
                close(1);
                int fd= open(args[prox_redireccion], O_CREAT | O_WRONLY | O_TRUNC, 0644);
                args[prox_redireccion-1]= NULL;
            }
            execv(inicio,args);
            printf("Unknown command\n");
        }
        else{
            wait();
        }
    }

    return 0;
}