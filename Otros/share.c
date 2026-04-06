#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

int main()
{
    const char *name = "SO";
    const int PAGESIZE = 4096;
    int fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, PAGESIZE);
    int *memoria = mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    int memoria_nocompartida = 12;


    if (fork() == 0)
    {
        *memoria = 10;
        memoria_nocompartida = 15;
    }
    else wait (0);

    printf("%d\n", *memoria);
    printf("%d\n", memoria_nocompartida);

    shm_unlink(name);

    return 0;
}