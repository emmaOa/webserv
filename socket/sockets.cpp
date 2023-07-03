/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouazzan <iouazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 14:54:32 by iouazzan          #+#    #+#             */
/*   Updated: 2023/06/23 19:32:43 by iouazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"
# include "../includes/socket.hpp"
# include "../includes/parsing_file_cnf.hpp"

std::deque<int> int_socket_srvs(void)
{
    std::deque<int> srvs(data_cnf->servers.size());
    unsigned long i = 0;
    while (i < data_cnf->servers.size())
    {
        srvs[i] = creat_socket(i);
        i++;
    }
    return srvs;
}

int creat_socket(int id)
{
    struct addrinfo hints, *bind_address;
    const char *host, *port;
    int socket_sv;
     int opt = 1;

    std::cout << "Configuring local address...\n";

    std::memset(&hints, 0, sizeof(hints));
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
        exit(1);
    }
    setsockopt(socket_sv, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    std::cout << "Binding socket to local address...\n";

    if (bind(socket_sv, bind_address->ai_addr, bind_address->ai_addrlen)) {
        std::cout << "bind failed\n";
        exit(1);
    }

    freeaddrinfo(bind_address);

    std::cout << "Listening...\n";
    if (listen(socket_sv, 10) < 0){
        std::cout << "listen failed\n";
        exit (1);
    }
    
    srvs_set sv;
    sv.socket= socket_sv;
    sv.port =atoi(port);
    sv.host =data_cnf->servers.at(id).at("host").at("null").at(0);

    servs.insert (std::pair<int, srvs_set> (socket_sv, sv));
    
    return socket_sv;
}
