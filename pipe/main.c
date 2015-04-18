#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "debug.h"

#define MAXLINE 256

int main(void)
{
    int n;
    int fd[2];
    pid_t pid;
    char line[MAXLINE];

    if (pipe(fd) < 0)
    {
        debug_error("pipe error");
        exit(-1);
    }
    if ((pid = fork()) < 0) 
    {
        debug_error("fork error");
        exit(-1);
    } 
    else if (pid > 0) 
    { /* parent */
        close(fd[0]);
        write(fd[1], "hello pipe\n", 12);
    }
    else { /* child */
        close(fd[1]);
        n = read(fd[0], line, MAXLINE);
        write(STDOUT_FILENO, line, n);
    }

    exit(0);
}



