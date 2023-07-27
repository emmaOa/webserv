/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clients.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namine <namine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 08:49:35 by iouazzan          #+#    #+#             */
/*   Updated: 2023/07/27 17:46:31 by namine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

int create_client(int sock)
{
    struct client_info *tmp = new client_info;
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
    tmp->is_done = -1;
    tmp->err = "null";
    tmp->is_boundary = 0;
    tmp->err_msg = "null";
    tmp->new_client = 0;
    tmp->current_position = 0;
    servs[sock].clts.insert(std::pair<int, client_info> (tmp->socket, *tmp));
    int socke = tmp->socket;
    delete tmp;
    std::cout << socke << std::endl;
    return socke;
}

int sock_s(int sock_c)
{
    std::map<int, srvs_set>::iterator it = servs.begin();
    std::map<int, struct client_info>::iterator it2 ;
    it = servs.begin();
    while (it != servs.end())
    {
        it2 = servs.at(it->first).clts.begin();
        while (it2 != servs.at(it->first).clts.end())
        {
            if (sock_c == servs.at(it->first).clts[it2->first].socket)
                return servs.at(it->first).socket;
            ++it2;
        }
        ++it;
    }
    return -1;
}


int wait_on_clients()
{
    unsigned long i = 0;
    fd_set re;


    int max_socket  = 0;
    std::map<int, srvs_set>::iterator it = servs.begin();
    std::map<int, struct client_info>::iterator it2;
    FD_ZERO (&re);
    fd_set wr;
    FD_ZERO (&wr);

    std::vector<int> vr;
    std::vector<int> vw;

    while (it != servs.end())
    {

        FD_SET(it->first, &re);
        if (it->first > max_socket)
                max_socket = it->first ;

        it2 = servs.at(it->first).clts.begin();
        while (it2 != servs.at(it->first).clts.end())
        {
            if (it2->second.is_done != 1){
                FD_SET (it2->first, &re);
                vr.push_back(it2->first);
            }
            else {
                FD_SET (it2->first, &wr);
                vw.push_back(it2->first);
            }
            if (it2->first > max_socket)
                max_socket = it2->first;
            ++it2;
        }
            ++it;
    }

    if (select(max_socket+1, &re, &wr, 0, 0) < 0){
        std::cout << "error 03\n";
        exit (1);
    }
    it = servs.begin();
    while (it != servs.end())
    {
        if (FD_ISSET(it->first, &re)) {

            create_client(servs.at(it->first).socket);
            return 0;
        }
        ++it;
    }

    int read_ret;
    char buffer[1024] = {0};
    i=0;
    while (i < vr.size())
    {
        if (FD_ISSET(vr[i], &re)){
            read_ret = read(vr[i], buffer, 1024);
            if (read_ret < 0){
                exit (1);
            }
            else{

                int s = sock_s(vr[i]);
                request_part(buffer, read_ret, vr[i], s);
            }
            return 0;
        }
        i++;
    }
    i=0;
    while (i < vw.size())
    {
        if (FD_ISSET(vw[i], &wr))
            response_part(vw[i], sock_s(vw[i]));
        i++;
    }
    return -1;
}
