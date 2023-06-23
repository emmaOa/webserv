/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fchanane <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 16:44:19 by fchanane          #+#    #+#             */
/*   Updated: 2023/06/12 16:44:20 by fchanane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <iostream>
#include <map>
#include <fcntl.h>
#include <unistd.h>

typedef struct Error_s
{
    int statusCode;
    std::string errorMsg;
}   Error_t;

class Request
{
    private :
        std::string method;
        std::string URI;
        std::string httpVer;
        std::map<std::string, std::string> headers;
        std::string body;
        Error_t status;
    public :
        int reqLineEnd;
        int headEnd;
        int bodyEnd;
        int statusReq;
        Request(void);
        Request(const std::string& method, const std::string& URI, const std::string& httpVer, const std::map<std::string, std::string>& headers, const std::string& body);
        Request(const Request& other);
        ~Request();
        Request& operator=(const Request& other);
        void parseHeaders(std::string& buffer);
        void parseContent(std::string& buffer);
        void parseRequestLine(std::string& requestLine);
        void parseHeaderLine(std::string& headerLine);
        void headerErrors(void);
        std::string getMethod(void);
        std::string getURI(void);
        std::string getBody(void){return this->body;}
        void requestPrinter(void);
        void printStatusBrowser(int clientFd);
        void printStatus(void);
};

#endif
