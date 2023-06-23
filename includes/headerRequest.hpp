/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headerRequest.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouazzan <iouazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 20:29:26 by fchanane          #+#    #+#             */
/*   Updated: 2023/06/23 19:53:32 by iouazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADERREQUEST_HPP
# define HEADERREQUEST_HPP

#include "request.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <set>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

typedef struct sockaddr_in address_t;

# define PORT 8080
# define BUFFER_SIZE 1024

# define CONN_MAX 128
# define UNFINISHED_REQ 0;
# define FINISHED_REQ 1;

extern std::vector<int> socketFd;

typedef struct matching_s
{
    std::string serverName;
    int clientBodyMax;
    std::string location;
    std::vector<std::string> allowedMethods;
    std::vector<int> clients;
}   matching_t;

int argCheck(int ac);
int exsDetection(char *av);
int fileOpening(char *av);
int programParser(int ac, char *av);
std::set<std::pair<std::string, int> > staticSockets();
int socketCreate(size_t number);
void    socketPrinter(std::set<std::pair<std::string, int> > sockets);
int socketBind(std::set<std::pair<std::string, int> > sockets, std::vector<address_t> &add);
int connectClient(void);
std::map<int, matching_t> abstractMatchingLocation(int serverFd);
int request_part(char *buffer, int clientFd, int serverFd);

#endif
