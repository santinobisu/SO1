/* Ej. 2. Supongamos que tenemos 2 threads o procesos que se ejecutan concurrentemente y tienen la
siguiente pinta:
Thread A
{
    a1;
    a2;
}

Thread B
{
    b1;
    b2;
}

Donde a1, a2, b1, b2 son instrucciones. Lo ´unico importante es que se necesita garantizar que a1 pase antes
que b2 y que b1 pase antes que a2. Implemente una soluci´on adecuada a este problema 

pthread_mutex_t lock;
pthread_cond_t b2_seguir, a2_seguir;
int turno_b2 = false, turno_a2 = false;

Thread A
{
    lock(&lock);
    a1;
    turno_b2 = true;
    unlock(&lock);
    signal(b2_seguir);
    lock(&lock);
    while (!(turno_a2)) wait(a2_seguir, &lock);
    a2;
    unlock(&lock);
}

Thread B
{
    lock(&lock);
    b1;
    turno_a2 = true;
    unlock(&lock);
    signal(a2_seguir);
    lock(&lock);
    while(!(turno_b2)) wait(b2_seguir, &lock);
    b2;
    unlock(&lock);
}

*/