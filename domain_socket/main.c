#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include "socket.h"

int main()
{
    pid_t pid;

    pid = fork();
    if (pid < 0)
    {
        perror("fork()");
    }
    else if (pid > 0)
    {
        int lsn_fd, apt_fd;
        char buf[128];
        int ret;

        printf("parent pid %d\n", getpid());
        lsn_fd = init_listen_server(REQUEST);
        apt_fd = accept_client(lsn_fd);
        ret = read(apt_fd, buf, 124);
        //ret = recv(apt_fd, buf, 124, 0);
        if (ret > 0)
        {
            buf[ret] = '\0';
            printf("%d, %s\n", getpid(), buf);
        }
    }
    else
    {
        int fd;
        
        printf("child pid %d\n", getpid());
        fd = connect_server(REQUEST);
        if (fd > 0)
        {
            write(fd, "I am child", strlen("I am child.") + 1);
            //send(fd, "I am child", strlen("I am child.") + 1, 0);
        }
    }

    return 0;
}

