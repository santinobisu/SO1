#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
/* int denom= 0;
void handler (int s){
    printf("outc\n");
    denom= 1;
}

int main()
{
    int r;
    signal(SIGFPE,handler);
    r= 1/denom; 
    printf("%d\n", r);  
 return 0;
} */

void handle(int s){
    printf("recibi la señal\n");
}

int main (){
    __pid_t pid= fork();
    signal(SIGFPE,handle);
    if(pid == 0){
        printf("Aca 1\n");
        sleep(3);
        printf("Aca 6\n");
    }
    else{
        printf("Aca 2\n");
        sleep(1);
        printf("Aca 3\n");
        kill(pid,SIGFPE);
        printf("Aca 4\n");
        wait();
        printf("Aca 5\n");
    }
    return 0;
}