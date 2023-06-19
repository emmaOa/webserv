/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clients.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouazzan <iouazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 08:49:35 by iouazzan          #+#    #+#             */
/*   Updated: 2023/06/19 09:47:28 by iouazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/socket.hpp"

struct client_info *clients::get_client(int sock)
{
    if (this->clts.find(sock) != this->clts.end()){
        return &this->clts.at(sock);
    }
    else {
        struct client_info *tmp;
        tmp = (struct client_info*) calloc(1, sizeof(struct client_info));
        if (!tmp) {
            std::cout << "error 01\n";
            exit (1);
        }
        tmp->address_length = sizeof(tmp->address);
        this->clts.insert (std::pair<int, client_info> (sock, *tmp));
        return tmp;
    }
}

void clients::drop_client(struct client_info *client)
{
    if (this->clts.find(client->socket) != this->clts.end()){
        this->clts.erase(client->socket);
        close(client->socket);
    }
    else {
        std::cout << "error 02\n";
        exit (1);
    }
}

fd_set clients::wait_on_clients(int server)
{
    fd_set reads;
    FD_ZERO (&reads);
    FD_SET(server, &reads);
    int max_socket = server;

    std::map<int, client_info>::iterator it = this->clts.begin();
    while (it != this->clts.end())
    {
        FD_SET (it->second.socket, &reads);
        if (it->second.socket > max_socket)
            max_socket = it->second.socket;
        ++it;
    }
    if (select(max_socket+1, &reads, 0, 0, 0) < 0){
        std::cout << "error 03\n";
        exit (1);
    }
    return reads;
}

void clients::send_404(struct client_info *client) {
    const char *c404 = "HTTP/1.1 400 Bad Request\r\n"
    "Connection: close\r\n"
    "Content-Length: 11\r\n\r\nBad Request";
    send(client->socket, c404, strlen(c404), 0);
    this->drop_client(client);
}

void clients::send_400(struct client_info *client) {
    const char *c400 = "HTTP/1.1 400 Bad Request\r\n"
    "Connection: close\r\n"
    "Content-Length: 11\r\n\r\nBad Request";
    send(client->socket, c400, strlen(c400), 0);
    this->drop_client(client);
}

std::string clients::get_client_address(client_info *ci)
{
    static char address_buffer[100];
    getnameinfo((struct sockaddr*)&ci->address,
    ci->address_length, \
    address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
    std::string resl(address_buffer);
    return resl;
}

clients::clients(/* args */)
{
}

clients::~clients()
{
}

