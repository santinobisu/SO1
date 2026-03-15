#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Ejercicio: Hacer una Shell

int main()
{
    while(1)
    {
        int largo = 255;
        char buff[largo];
        fgets(buff, largo, stdin);
        buff[strcspn(buff,"\n")] = 0;
        char *token = strtok(buff, " "); //token = comando/path
        //if(strcmp(token, "exit()"))break;
        char inicio[largo];
        strcpy(inicio, "/bin/");
        char *final = strcat(inicio, token);
        char* args[largo];
        int i = 0;
        args[i]= token;
        i++;
        token = strtok(NULL, " ");  
        while (token != NULL)
        {
            args[i] = token;
            printf("%s\n", args[i]);
            token = strtok(NULL, " ");
            i++;
        }
        args[i]= NULL;
        __pid_t pid= fork();
        if(pid== 0){
            printf("%s",final);
            execv(inicio,args);
        }
        else{
            wait();
        }
    }

    return 0;
}