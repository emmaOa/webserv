#ifndef SOCKET_HPP
# define SOCKET_HPP
# define ROOT /
# define CGI_PATH_PY /usr/bin/python3
# define CGI_PATH_SH /bin/bash
# define CGI_EXT_1 .py
# define CGI_EXT_2 .sh


struct client_info {
    socklen_t address_length;
    struct sockaddr_storage address;
    int socket;
    int socket_srv;
    char request[2048];
    std::string fd_name;
    int is_done;
    std::map<std::string, std::string> request_map;
    std::string err;
    int is_boundary;
    std::string err_msg;
    std::string location;
    int new_client;
    int current_position;
    std::string path;
};

typedef struct s_srvs_set {
    int socket;
    int port;
    std::string host;
    std::map<int, struct client_info> clts;
    struct sockaddr_in serv_addr;
}      srvs_set;

extern std::map<int, srvs_set> servs;

int create_socket(int id);
std::deque<int> int_socket_srvs(void);
int wait_on_clients(int server);
std::deque<int> int_socket_srvs(void);
int create_client(int sock);
int check_request(fd_set reads, std::vector<int> v);
int check_response(int sock);

#endif
