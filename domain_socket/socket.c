#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>


#include "socket.h"


int init_listen_server(const char *domain_name)
{
    int lsn_fd;
    struct sockaddr_un srv_addr;
    int ret;

    //create socket to bind local IP and PORT
    lsn_fd = socket(PF_UNIX, SOCK_STREAM, 0);
    if(lsn_fd < 0)
    {
      perror("can't create communication socket!");
      return -1;
    }
    
    //create local IP and PORT
    srv_addr.sun_family = AF_UNIX;
    strncpy(srv_addr.sun_path, domain_name, sizeof(srv_addr.sun_path) - 1);
    unlink(domain_name);
    
    //bind sockfd and sockaddr
    ret = bind(lsn_fd, (struct sockaddr*)&srv_addr, sizeof(srv_addr));
    if(ret == -1)
    {
      perror("can't bind local sockaddr!");
      close(lsn_fd);
      unlink(domain_name);
      return -1;
    }
    
    //listen lsn_fd, try listen 1
    ret = listen(lsn_fd, 1);
    if(ret == -1)
    {
      perror("can't listen client connect request");
      close(lsn_fd);
      unlink(domain_name);
      return -1;
    }
    return lsn_fd;
}

int accept_client(int lsn_fd)
{
    int apt_fd;
    struct sockaddr_un clt_addr;
    socklen_t clt_len;

    clt_len = sizeof(clt_addr);

    apt_fd = accept(lsn_fd, (struct sockaddr*)&clt_addr, &clt_len);
    if(apt_fd < 0)
    {
        return -1; 
    }

    return apt_fd;
}

int connect_server(const char *domain_name)
{
    int connect_fd;
    struct sockaddr_un srv_addr;
    int ret;

    //create client socket
    connect_fd = socket(PF_UNIX, SOCK_STREAM, 0);
    if(connect_fd < 0)
    {
        perror("client create socket failed");
        return -1;
    }

    //set server sockaddr_un
    srv_addr.sun_family = AF_UNIX;
    strcpy(srv_addr.sun_path, domain_name);

    //connect to server
    ret = connect(connect_fd, (struct sockaddr*)&srv_addr, sizeof(srv_addr));
    if(ret == -1)
    {
        char buf[128];
        sprintf(buf, "pid %d, %s", getpid(), "connect to server failed!");
        //printf("pid %d\n", getpid());
        perror(buf);
        close(connect_fd);
        unlink(domain_name);
        return -1;
    }

    return connect_fd;
}

