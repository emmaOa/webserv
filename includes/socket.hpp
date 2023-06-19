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


int srvs(void);
int clients(void);
std::deque<int> creat_socket(void);
std::map<int, srvs_set> servs;

struct srvs_set {
    int socket;
    std::string first_lct;
    int port;
    std::string host;
    std::deque<int> clients_sock;
    struct sockaddr_in serv_addr;
};

class srv_socket
{
    private:
        struct sockaddr_in serv_addr;
        int id_srv;
    public:
        srv_socket();
        srv_socket(int id);
        sockaddr_in get_srv_add();
        srv_socket(const srv_socket &obj);
        ~srv_socket();
};

struct client_info {
    socklen_t address_length;
    struct sockaddr_storage address;
    int socket;
    char request[2048];
    int received;
};

class clients
{
    private:
        std::map<int, client_info> clts;
        client_info info;
    public:
        clients(/* args */);
        client_info *get_client(int sock);
        void drop_client(client_info *client);
        std::string get_client_address(client_info *ci);
        fd_set wait_on_clients(int server);
        void send_400(struct client_info *client);
        void clients::send_404(struct client_info *client);
        ~clients();
};






#endif
