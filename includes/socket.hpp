#ifndef SOCKET_HPP
# define SOCKET_HPP
# define ROOT /
# define CGI_PATH_PY /usr/bin/python3
# define CGI_PATH_PHP /Users/iouazzan/Desktop/back/php-cgi;
# define CGI_EXT_1 .py
# define CGI_EXT_2 .php


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
    unsigned long len_bound;
    int is_ex_cgi;
    std::string file_cgi;
    std::string type_cgi;
    long first_time_cgi;
    std::string boundry;
    std::string loopDetected;
    int  sizeOfReresource;
    int rest;
    int pid;
    std::string header_php;
};

typedef struct s_srvs_set {
    int socket;
    int port;
    std::string host;
    std::string s_name;
    std::map<int, struct client_info> clts;
    struct sockaddr_in serv_addr;
}      srvs_set;

extern std::map<int, srvs_set> servs;
extern int exec_err;

int create_socket(int id);
std::deque<int> int_socket_srvs(void);
int wait_on_clients();
std::deque<int> int_socket_srvs(void);
int create_client(int sock);
int check_request(fd_set reads, std::vector<int> v);
int check_response();

#endif
