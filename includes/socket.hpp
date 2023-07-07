#ifndef SOCKET_HPP
# define SOCKET_HPP

struct client_info {
    socklen_t address_length;
    struct sockaddr_storage address;
    int socket;
    int socket_srv;
    char request[2048];
    std::string fd_name;
    std::map<std::string, std::string> request_map;
    int err;
    std::string err_msg;
};

typedef struct s_srvs_set {
    int socket;
    int port;
    std::string host;
    std::map<int, struct client_info> clts;
    struct sockaddr_in serv_addr;
    
}               srvs_set;

extern std::map<int, srvs_set> servs;

int create_socket(int id);
std::deque<int> int_socket_srvs(void);
int wait_on_clients(int server);
std::deque<int> int_socket_srvs(void);
int create_client(int sock);
int check_request(fd_set reads, int server);
int check_response(int sock);

#endif
