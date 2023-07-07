/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clients.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouazzan <iouazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 08:49:35 by iouazzan          #+#    #+#             */
/*   Updated: 2023/06/24 02:36:22 by iouazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"
# include "../includes/parsing_file_cnf.hpp"
# include "../includes/socket.hpp"

int creat_client(int sock)
{
    struct client_info *tmp;
    tmp = (struct client_info*) calloc(1, sizeof(struct client_info));
    if (!tmp) {
        std::cout << "error 01\n";
        exit (1);
    }
    tmp->address_length = sizeof(tmp->address);
    tmp->socket = accept(sock, (struct sockaddr*) &(tmp->address), &(tmp->address_length));
    if (tmp->socket < 0){
        std::cout << "accept field\n";
       return -1; 
    }
    tmp->socket_srv = sock;
    tmp->fd_name = "null";
    tmp->err = -1;
    tmp->err_msg = "null";
    servs[sock].clts.insert(std::pair<int, client_info> (tmp->socket, *tmp));
    // std::cout << servs[0].clts[0].socket << std::endl;
    return tmp->socket;
}

int check_request(fd_set reads, int server)
{
    unsigned long i = 0;

    while (i < servs.at(server).clts.size())
    {
        if (FD_ISSET(servs.at(server).clts[i].socket, &reads)){
            return servs.at(server).clts[i].socket;
        }
        i++;
    }
    return -1;
}

int check_response(int sock)
{
    fd_set wr;
    FD_ZERO (&wr);

    int max = sock;

    timeval tm;
    tm.tv_sec = 10;
    tm.tv_usec = 0;
    
    FD_SET (sock, &wr);
    if (select(max+1, 0, &wr, 0, &tm) < 0){
        std::cout << "error 04\n";
        exit (1);
    }
    if (FD_ISSET(sock, &wr)){
        return sock;
    }
    return -1;
}

int wait_on_clients(int server)
{
    unsigned long i = 0;
    fd_set re;

    timeval tm;
    tm.tv_sec = 0;
    tm.tv_usec = 1000;

    FD_ZERO (&re);
    FD_SET(server, &re);
    int max_socket = server;
    // std::cout << server << " <<------\n";
    while (i < servs.at(server).clts.size())
    {
        FD_SET (servs.at(server).clts[i].socket, &re);
        if (servs.at(server).clts[i].socket > max_socket)
            max_socket = servs.at(server).clts[i].socket;
        i++;
    }

    if (select(max_socket+1, &re, 0, 0, &tm) < 0){
        std::cout << "error 03\n";
        exit (1);
    }

    if (FD_ISSET(server, &re)) {
        try {
            creat_client(servs.at(server).socket);
        }
        catch (const std::out_of_range& e) {
            std::cerr << "Exception srv_socket default: " << e.what() << std::endl;
        }
        return -1;
    }
    int val = check_request(re, server);
    return val;  
}


