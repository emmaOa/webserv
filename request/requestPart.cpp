/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestPart.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouazzan <iouazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 19:03:47 by fchanane          #+#    #+#             */
/*   Updated: 2023/06/23 20:40:39 by iouazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/headerRequest.hpp"

int request_part(char *buffer, int clientFd, int serverFd)
{
    std::map<int, Request> Events;

    (void) serverFd;
    std::string strBuff(buffer, strlen(buffer));
    Events[clientFd] = Request();// created a request object for this client
    if (!Events[clientFd].headEnd)
    {
        Events[clientFd].parseHeaders(strBuff);
    }
    else
        Events[clientFd].parseContent(strBuff);
    Events[clientFd].requestPrinter(); //this one to debug request parsing mostly its headers
    Events[clientFd].headerErrors();
    //Events[clientFd].printStatus(); //this one to debug request status after parsing
    //std::cout<<strBuff; // this line is to print raw request just to compare it with the parsing
    if (Events[clientFd].bodyEnd)
    {
        //std::cout<<"Request parsing END"<<std::endl;
        return 1;
    }
    return 0;
}
