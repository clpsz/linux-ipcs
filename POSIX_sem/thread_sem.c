#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "debug.h"

sem_t sem;

void *thread(void *dummy)
{
    sleep(1);
    debug_info("new thread posting\n");
    sem_post(&sem);

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t pid;
    // not share within process, initial value 0
    pthread_create(&pid, NULL, thread, NULL);
    sem_init(&sem, 0, 0);

    sem_wait(&sem);
    debug_info("main thread wait succeed\n");
    sem_post(&sem);

    sem_destroy(&sem);

    pthread_join(pid, NULL);

    return 0;
}

