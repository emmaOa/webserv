/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouazzan <iouazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 14:54:32 by iouazzan          #+#    #+#             */
/*   Updated: 2023/06/19 09:55:05 by iouazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/socket.hpp"

// void error(char *msg)
// {
//     perror(msg);
//     exit(1);
// }

int clients(void)
{
    
    return 0;
}

int srvs(void)
{
    srv_socket *srv = new srv_socket;
    struct sockaddr_in cli_add;
    char buffer[256];
    //  std::cout << srv.serv_addr.sin_addr.s_addr << "  : heereeee \n";
    int opt = 1, n;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        std::cout << "open failed socket\n";
        exit (1);
    }
    if (setsockopt(sockfd, SOL_SOCKET,  SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {  
            perror("setsockopt");  
            exit(EXIT_FAILURE);  
    }
    const sockaddr_in &tmp = srv->get_srv_add();
    if (bind(sockfd, (struct sockaddr *) &tmp, sizeof(srv->get_srv_add())) < 0) {
        std::cout << "bind failed\n";
        exit (1);
    }

    if (listen(sockfd,10) < 0){
        std::cout << "listen failed\n";
        exit (1);
    }
    socklen_t cliant = sizeof(cli_add);
    
    int newsockfd = accept(sockfd, (struct sockaddr *) &cli_add, &cliant);
    if (newsockfd < 0){
        std::cout << "open failed socket cliant\n";
        exit (1);
    }
    bzero(buffer,256);
    n = read(newsockfd,buffer,255);
    if (n < 0) {
        std::cout << "ERROR reading from socket\n";
        exit (1);
    }
    printf("Here is the message: %s\n",buffer); 

    n = write(newsockfd,"I got your message",18);
    if (n < 0) {
        std::cout << "ERROR writing from socket\n";
        exit (1);
    }
    // setsockopt(sockfd, SOL_SOCKET,
    //                SO_REUSEADDR | SO_REUSEPORT, &opt,
    //                sizeof(opt));
    return 0;
}
