#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "debug.h"


#define SEMKEY 1

#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
    /* union semun is defined by including <sys/sem.h> */
#else
    /* according to X/OPEN we have to define it ourselves */
    union semun {
        int val;                    /* value for SETVAL */
        struct semid_ds *buf;       /* buffer for IPC_STAT, IPC_SET */
        unsigned short int *array;  /* array for GETALL, SETALL */
        struct seminfo *__buf;      /* buffer for IPC_INFO */
    };
#endif


void print0(size_t count)
{
    int i;

    for (i = 0; i < count; i++)
    {
        fprintf(stderr, "0");
		usleep(10);
    }
    fprintf(stderr, "\n");
}

void print1(size_t count)
{
    int i;

    for (i = 0; i < count; i++)
    {
        fprintf(stderr, "1");
		usleep(10);
    }
    fprintf(stderr, "\n");
}

int semid_init(int *semid)
{
    int sid;

    sid = semget(SEMKEY, 0, 0666);
    if (sid < 0)
    {
        sid = semget(SEMKEY, 1, IPC_CREAT| IPC_EXCL | 0666);
        if (sid < 0)
        {
            perror("semget: ");
            return -1;
        }
    }
    *semid = sid;

    return 0;
}

int semid_destory(int semid)
{
    return semctl(semid, 0, IPC_RMID);
}


int sem_action(int semid, int val)
{
    struct sembuf sb;
    int ret;

    sb.sem_num = 0;
    sb.sem_op = val;
    sb.sem_flg = 0;

    ret = semop(semid, &sb, 1);
    if (ret < 0)
    {
        perror("semop: ");
        return -1;
    }

    return 0;
}

int sem_add(int semid, int val)
{
#if defined(USING_SEM)
    return sem_action(semid, val);
#else
    return 0;
#endif
}

int sem_sub(int semid, int val)
{
#if defined(USING_SEM)
    return sem_action(semid, -1*val);
#else
    return 0;
#endif
}


void parent()
{
    int semid;
    int ret;

    ret = semid_init(&semid);
    if (ret < 0)
    {
        exit(-1);
    }

	union semun sem_union;
	sem_union.val = 1;
	if (semctl(semid, 0, SETVAL, sem_union) == -1) 
		exit(-1);

    //sem_add(semid, 1);

    //while (1)
    {
        sem_sub(semid, 1);
        print0(100);
        sem_add(semid, 1);
    }

	sleep(5);
	
	if (semctl(semid, 0, IPC_RMID, sem_union) == -1)
     	exit(-1);

	
}

void child()
{
    int semid;

    semid_init(&semid);

    //while (1)
    {
        sem_sub(semid, 1);
        print1(100);
        sem_add(semid, 1);
    }
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



