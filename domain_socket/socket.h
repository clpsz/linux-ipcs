#ifndef __SOCKET_H__
#define __SOCKET_H__


#define REQUEST "/tmp/test_request.domain"

int init_listen_server(const char *domain_name);
int accept_client(int lsn_fd);
int connect_server(const char *domain_name);

#endif

