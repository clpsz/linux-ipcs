#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "debug.h"


#define MSGKEY 1

typedef struct
{
    int type;
    char buf[512];
} MSGBUF;


void parent()
{
    int msgid;
    int ret;
    MSGBUF msg;

    msgid = msgget(MSGKEY, IPC_CREAT| IPC_EXCL | 0666);
    if (msgid < 0)
    {
        perror("msgget: ");
        msgid = msgget(MSGKEY, 0666);
        goto remove;
    }
    usleep(100); // wait child entering receiving status

    msg.type = 0;
    strcpy(msg.buf, "I am parent");

    ret = msgsnd(msgid, &msg, sizeof(msg), IPC_NOWAIT); 
    if (ret < 0)
    {
        perror("msgsnd: ");
        goto remove;
    }

remove:
    ret = msgctl(msgid, IPC_RMID, NULL);
    if (ret < 0)
    {
        perror("msgctl: ");
    }
}

void child()
{
    int msgid;
    int ret;
    MSGBUF msg;

    msgid = msgget(MSGKEY, 0666);
    if (msgid < 0)
    {
        perror("msgget: ");
    }

    ret = msgrcv(msgid, &msg, sizeof(msg), 0, 0);
    if (ret < 0)
    {
        perror("msgrcv: ");
        return;
    }
    debug_info("msg is: %s\n", msg.buf);
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



