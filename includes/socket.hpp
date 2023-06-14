#ifndef SOCKET_HPP
# define SOCKET_HPP

# include "webserv.hpp"

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>


int socket(void);

class srv_socket
{
    private:
        struct sockaddr_in serv_addr;
        int id_srv;
    public:
        srv_socket();
        srv_socket(int port);
        sockaddr_in get_srv_add();
        srv_socket(const srv_socket &obj);
        ~srv_socket();
};




#endif
