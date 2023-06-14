/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   srv.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouazzan <iouazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 18:52:27 by iouazzan          #+#    #+#             */
/*   Updated: 2023/06/14 17:52:16 by iouazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/socket.hpp"
# include "../includes/parsing_file_cnf.hpp"

srv_socket::srv_socket(/* args */)
{
    // serv_addr = new  sockaddr_in;
    this->id_srv = 0;
    this->serv_addr.sin_family = AF_INET;
    try {
        std::string portString = data_cnf->servers.at(id_srv).at("port").at("null").at(0);
        this->serv_addr.sin_port = htons(std::stoi(portString));
    
        std::string ipAddress = data_cnf->servers.at(id_srv).at("host").at("null").at(0);
        this->serv_addr.sin_addr.s_addr = inet_addr(ipAddress.c_str());
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Exception srv_socket default: " << e.what() << std::endl;
    }
    // if (this->serv_addr.sin_addr.s_addr == htonl(INADDR_LOOPBACK))
        // std::cout << this->serv_addr.sin_addr.s_addr << "  : heereeee \n";
}

srv_socket::srv_socket(int id)
{
    id_srv = id;
    this->serv_addr.sin_family = AF_INET;
    try{
        std::string portString = data_cnf->servers.at(id_srv).at("port").at("null").at(0);
        this->serv_addr.sin_port = htons(std::stoi(portString));
    
        std::string ipAddress = data_cnf->servers.at(id_srv).at("host").at("null").at(0);
        this->serv_addr.sin_addr.s_addr = inet_addr(ipAddress.c_str());
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Exception srv_socket id: " << e.what() << std::endl;
    }

    
}

sockaddr_in srv_socket::get_srv_add()
{
    return this->serv_addr;
}

srv_socket::srv_socket(const srv_socket &obj)
{
    *this = obj;
}

srv_socket::~srv_socket()
{
}
