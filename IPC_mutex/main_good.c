#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "debug.h"


#define SHMKEY 2
int shmid;
char *addr;


pthread_mutex_t *p_mutex;
pthread_mutexattr_t mutex_attr;

void sigint_handler(int signal)
{
    int ret;

    ret = shmctl(shmid, IPC_RMID, NULL);
    if (ret < 0)
    {
        perror("shmctl");
    }
    pthread_mutexattr_destroy(&mutex_attr);
}


void print0(size_t count)
{
    int i;

    for (i = 0; i < count; i++)
    {
        fprintf(stderr, "0");
    }
    fprintf(stderr, "\n");
}

void print1(size_t count)
{
    int i;

    for (i = 0; i < count; i++)
    {
        fprintf(stderr, "1");
    }
    fprintf(stderr, "\n");
}

void parent()
{
    int i = 100;

    signal(SIGINT, sigint_handler);

    shmid = shmget(SHMKEY, 0, 0666);
    if (shmid < 0)
    {
        shmid = shmget(SHMKEY, 4096, IPC_CREAT | IPC_EXCL | 0666);
        if (shmid < 0)
        {
            perror("parent shmget");
            return;
        }
    }

    addr = shmat(shmid, 0, 0);
    if (addr == (void *)-1)
    {
        perror("shmat");
        return;
    }

    p_mutex = (pthread_mutex_t *)addr;
    pthread_mutex_init(p_mutex, &mutex_attr);


    while(i--)
    {
        pthread_mutex_lock(p_mutex);
        print0(100);
        pthread_mutex_unlock(p_mutex);
    }
}

void child()
{
    int i = 100;
    int shmid;
    char *addr;

    shmid = shmget(SHMKEY, 0, 0666);
    if (shmid < 0)
    {
        perror("child shmget");
        return;
    }

    addr = shmat(shmid, 0, 0);
    p_mutex = (pthread_mutex_t *)addr;

    while(i--)
    {
        pthread_mutex_lock(p_mutex);
        print1(100);
        pthread_mutex_unlock(p_mutex);
    }
}


int main(void)
{
    pid_t pid;
    int status;

    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
    //pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_PRIVATE);

    debug_info("sizeof mutex is: %ld\n", sizeof(pthread_mutex_t));

    if ((pid = fork()) < 0) 
    {
        debug_error("fork error");
        exit(-1);
    } 
    else if (pid > 0) 
    { /* parent */
        int ret;
        parent();
        wait(&status);

        ret = shmctl(shmid, IPC_RMID, NULL);
        if (ret < 0)
        {
            perror("shmctl");
        }
        pthread_mutexattr_destroy(&mutex_attr);
    }
    else
    { /* child */
        usleep(200);
        child();
    }

    exit(0);
}

