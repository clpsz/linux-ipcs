#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "debug.h"


#define SHMKEY 1


void parent()
{
    int shmid;
    int ret;
    char *addr;

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
    if ((int)addr == -1)
    {
        perror("shmat");
        return;
    }

    sprintf(addr, "I am parent");

    usleep(500);
    ret = shmctl(shmid, IPC_RMID, NULL);
    if (ret < 0)
    {
        perror("shmctl");
    }
}

void child()
{
    int shmid;
    char *addr;
    char buf[1024];

    shmid = shmget(SHMKEY, 0, 0666);
    if (shmid < 0)
    {
        perror("child shmget");
        return;
    }
    
    addr = shmat(shmid, 0, 0);
    
    memcpy(buf, addr, 1024);
    debug_info("buf is: %s\n", buf);
}

int main(void)
{
    pid_t pid;
    int status;

    if ((pid = fork()) < 0) 
    {
        debug_error("fork error");
        exit(-1);
    } 
    else if (pid > 0) 
    { /* parent */
        parent();
        wait(&status);
    }
    else
    { /* child */
        child();
    }

    exit(0);
}

