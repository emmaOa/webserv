/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestPart.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouazzan <iouazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 19:03:47 by fchanane          #+#    #+#             */
/*   Updated: 2023/06/24 03:01:20 by iouazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"
# include "../includes/parsing_file_cnf.hpp"
# include "../includes/socket.hpp"
#include "../includes/headerRequest.hpp"

int request_part(char *buffer, int clientFd, int serverFd)
{
    // std::map<int, Request> Events;

    (void) clientFd;
    (void) serverFd;
    std::string strBuff(buffer, strlen(buffer));
    Request *clientRe = new Request;
    servs[serverFd].clts[clientFd].clientReq = clientRe;
    // std::cout << servs[serverFd].clts[clientFd].clientReq->headEnd << "----<<";
    servs[serverFd].clts[clientFd].clientReq->parseHeaders(strBuff);
    if (!servs[serverFd].clts[clientFd].clientReq->headEnd)
    {
        servs[serverFd].clts[clientFd].clientReq->parseHeaders(strBuff);
    }
    else
        servs[serverFd].clts[clientFd].clientReq->parseContent(strBuff);
    servs[serverFd].clts[clientFd].clientReq->requestPrinter(); //this one to debug request parsing mostly its headers
    servs[serverFd].clts[clientFd].clientReq->headerErrors();
    servs[serverFd].clts[clientFd].clientReq->printStatus();
    //servs[serverFd].clts[clientFd].clientReq->printStatus(); //this one to debug request status after parsing
    //std::cout<<strBuff; // this line is to print raw request just to compare it with the parsing
    if (servs[serverFd].clts[clientFd].clientReq->bodyEnd)
    {
        //std::cout<<"Request parsing END"<<std::endl;
        return 1;
    }
    return 0;
}
