/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouazzan <iouazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 14:54:32 by iouazzan          #+#    #+#             */
/*   Updated: 2023/06/14 16:40:17 by iouazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/socket.hpp"



int socket(void)
{
    srv_socket *srv = new srv_socket;
    //  std::cout << srv.serv_addr.sin_addr.s_addr << "  : heereeee \n";
    // int opt = 1;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        std::cout << "open failed socket\n";
        exit (1);
    }
    if (bind(sockfd, (struct sockaddr *) &srv->get_srv_add(), sizeof(srv->get_srv_add())) < 0) {
        std::cout << "bind failed\n";
        exit (1);
    }
    // setsockopt(sockfd, SOL_SOCKET,
    //                SO_REUSEADDR | SO_REUSEPORT, &opt,
    //                sizeof(opt));
    return 0;
}
