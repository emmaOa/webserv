# include "../includes/webserv.hpp"

int check_creat_srv(std::vector<std::pair<std::string, std::string> > &v, std::vector<std::string > &v_name, std::string port, std::string host, std::string s_name)
{
    for (unsigned long i = 0; i < v.size(); i++)
    {
        if (v[i].first.compare(port) == 0 && v[i].second.compare(host) == 0 && v_name[i].compare(s_name) != 0){
            std::cout << "the same configuration server twice \n";
            exit(0);
        }
    }
    return 0;
}

std::deque<int> int_socket_srvs(void)
{
    std::deque<int> srvs(data_cnf->servers.size());
    unsigned long i =0;
    std::vector<std::pair<std::string, std::string> > v;
    
    std::vector<std::string > v_name;
    while (i < data_cnf->servers.size())
    {
        if (check_creat_srv(v, v_name, data_cnf->servers.at(i).at("port").at("null").at(0), data_cnf->servers.at(i).at("host").at("null").at(0), data_cnf->servers.at(i).at("server_name").at("null").at(0)) == 0){
            std::cout << "----creat server\n";
            srvs[i] = create_socket(i);
            if (srvs[i] < 0)
                exit (0);
            v.push_back(std::make_pair(data_cnf->servers.at(i).at("port").at("null").at(0), data_cnf->servers.at(i).at("host").at("null").at(0)));
            v_name.push_back(data_cnf->servers.at(i).at("server_name").at("null").at(0));
        }
        i++;
    }
    return srvs;
}

int create_socket(int id)
{
    struct addrinfo hints, *bind_address;
    const char *host, *port;
    int socket_sv;
     int opt = 1;

    std::cout << "Configuring local address...\n";

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    host = (data_cnf->servers.at(id).at("host").at("null").at(0)).c_str();
    port = (data_cnf->servers.at(id).at("port").at("null").at(0)).c_str();
    getaddrinfo(host, port, &hints, &bind_address);

    std::cout << "Creating socket...\n";

    socket_sv = socket(bind_address->ai_family,
        bind_address->ai_socktype, bind_address->ai_protocol);
    if (socket_sv < 0) {
        std::cout << "open failed socket\n";
        return -1;
    }
    setsockopt(socket_sv, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    std::cout << "Binding socket to local address...\n";

    if (bind(socket_sv, bind_address->ai_addr, bind_address->ai_addrlen)) {
        std::cout << "bind failed\n";
        close(socket_sv);
        return -1;
    }

    freeaddrinfo(bind_address);

    std::cout << "Listening...\n";
    if (listen(socket_sv, 128) < 0){
        std::cout << "listen failed\n";
        return -1;
    }
    
    srvs_set sv;
    sv.socket= socket_sv;
    sv.port =atoi(port);
    sv.host =data_cnf->servers.at(id).at("host").at("null").at(0);
    if (data_cnf->servers.at(id).at("server_name").at("null").size() > 0)
        sv.s_name = data_cnf->servers.at(id).at("server_name").at("null").at(0);
    else
        sv.s_name = "null";

    servs.insert (std::pair<int, srvs_set> (socket_sv, sv));
    
    return socket_sv;
}
