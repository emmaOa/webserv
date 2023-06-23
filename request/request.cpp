/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouazzan <iouazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 16:42:09 by fchanane          #+#    #+#             */
/*   Updated: 2023/06/23 19:53:13 by iouazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/headerRequest.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

Request::Request(void)
{
    status.statusCode = 200;
    status.errorMsg = "OK";
    reqLineEnd = 0;
    headEnd = 0;
    bodyEnd = 0;
    statusReq = UNFINISHED_REQ;
    //std::cout<<"Default constrcutor called"<<std::endl;
}

Request::Request(const std::string& method, const std::string& URI, const std::string& httpVer, const std::map<std::string, std::string>& headers, const std::string& body)
    : method(method), URI(URI), httpVer(httpVer), headers(headers), body(body)
{
    reqLineEnd = 0;
    headEnd = 0;
    bodyEnd = 0;
    statusReq = UNFINISHED_REQ;
   // std::cout<<"Constrcutor called"<<std::endl;
}

Request::Request(const Request& other)
    : method(other.method), URI(other.URI), httpVer(other.httpVer), headers(other.headers), body(other.body), reqLineEnd(other.reqLineEnd), headEnd(other.headEnd), bodyEnd(other.bodyEnd)
{
    //std::cout<<"Copy constrcutor called"<<std::endl;
}

Request::~Request()
{
    //std::cout<<"Destructor called"<<std::endl;
}

Request& Request::operator=(const Request& other)
{
    if (this != &other) {
        method = other.method;
        URI = other.URI;
        httpVer = other.httpVer;
        headers = other.headers;
        body = other.body;
        reqLineEnd = other.reqLineEnd;
        headEnd = other.headEnd;
        bodyEnd = other.bodyEnd;
        statusReq = other.statusReq;
    }
    //std::cout<<"Assignment constrcutor called"<<std::endl;
    return *this;
}

std::string Request::getMethod(void)
{
    return (this->method);
}

std::string Request::getURI(void)
{
    return (this->URI);
}

void Request::requestPrinter(void)
{
    std::cout<<">>>>>>>>>> Starting Printer <<<<<<<<<<\n"<<std::endl;
    std::cout<<"Method = "<<this->method<<std::endl;
    std::cout<<"URI = "<<this->URI<<std::endl;
    std::cout<<"httpVer = "<<this->httpVer<<std::endl;
    std::map<std::string, std::string>::const_iterator it;
    for (it = this->headers.begin(); it != this->headers.end(); ++it) {
        std::cout << it->first << " = " << it->second << std::endl;
    }
    std::cout<<"Body = \n"<<this->body<<std::endl;
    std::cout<<"\n>>>>>>>>>> Ending Printer <<<<<<<<<<\n"<<std::endl;

}

void Request::printStatus(void)
{
    std::cout<<">>>>>>>>>> REQUEST STATUS <<<<<<<<<<"<<std::endl;
    std::cout<<"\nSTATUS CODE : "<<this->status.statusCode<<std::endl;
    std::cout<<"STATUS MSG : "<<this->status.errorMsg<<std::endl;
    std::cout<<"\nEND.\n"<<std::endl;
}

void Request::parseRequestLine(std::string& requestLine)
{
    int pos;
    pos = requestLine.find(" ");
    std::string method = requestLine.substr(0, pos);
    this->method = method;
    requestLine = requestLine.substr(pos + 1);
    pos = requestLine.find(" ");
    std::string URI = requestLine.substr(0, pos);
    this->URI = URI;
    requestLine = requestLine.substr(pos + 1);
    std::string httpVer = requestLine;
    this->httpVer = httpVer;
    this->reqLineEnd = 1;
}

void Request::parseHeaderLine(std::string& headerLine)
{
    int pos;
    pos = headerLine.find(" ");
    std::string name = headerLine.substr(0, pos - 1);
    std::string value = headerLine.substr(pos + 1);
    this->headers.insert(std::make_pair(name, value));
}

void Request::parseHeaders(std::string& buffer)
{
    size_t pos;
    pos = buffer.find("\r\n\r\n");
    std::string headerPart = buffer.substr(0, pos);
    std::string bodyPart = buffer.substr(pos + 4);
    this->body = bodyPart;
    pos = headerPart.find("\r\n");
    std::string requestLine = headerPart.substr(0, pos);
    headerPart = headerPart.substr(pos + 2);
    parseRequestLine(requestLine);
    pos = headerPart.find("\r\n");
    while (pos != std::string::npos)
    {
        std::string headerLine = headerPart.substr(0, pos);
        parseHeaderLine(headerLine);
        headerPart = headerPart.substr(pos + 2);
        pos = headerPart.find("\r\n");
    }
    if (!headerPart.empty())
        parseHeaderLine(headerPart);
    this->headEnd = 1;
}

void Request::headerErrors(void)
{
    //allowed method but need upgrade and change after MATCHING LOCATION
    if (this->method != "GET" && this->method != "DELETE" && this->method != "POST")
    {
        // i should check allowed methods also
        this->status.statusCode = 501;
        this->status.errorMsg = "Not Implemented";
        return;
    }
    //allowed characters in URI
    std::string allowedChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789:/?#-._~[]@!$&'()*+,;=";
    for (size_t i = 0; i < this->URI.length(); i++)
    {
        if (allowedChars.find(this->URI[i]) == std::string::npos)
        {
            this->status.statusCode = 400;
            this->status.errorMsg = "Bad Request";
            return;
        }
    }
    // URI more than 2048 but need more testing
    if (this->URI.length() > 2048)
    {
        this->status.statusCode = 414;
        this->status.errorMsg = "Request-URI Too Long";
        return;
    }
    // HTTP version need to be tested btw
    if (this->httpVer != "HTTP/1.1")
    {
        std::cout<<"this is "<<this->httpVer<<std::endl;
        this->status.statusCode = 400;
        this->status.errorMsg = "Bad Request";
        return;
    }
    // Host should be present
    std::map<std::string, std::string>::iterator itH = this->headers.find("Host");
    if (itH == this->headers.end())
    {
        this->status.statusCode = 400;
        this->status.errorMsg = "Bad Request";
        return;
    }
    // Transfer-Encoding and Content-Length Errors with POST
    if (this->method == "POST")
    {
        std::map<std::string, std::string>::iterator itT = this->headers.find("Transfer-Encoding");
        std::map<std::string, std::string>::iterator itC = this->headers.find("Content-Length");

        if (itT == this->headers.end())
        {
            this->status.statusCode = 400;
            this->status.errorMsg = "Bad Request";
            return ;
        }
        else
        if (itT->second != "chunked")
        {
            this->status.statusCode = 501;
            this->status.errorMsg = "Not Implemented";
            return ;
        }
        if (itC == this->headers.end())
        {
            this->status.statusCode = 400;
            this->status.errorMsg = "Bad Request";
            return ;
        }
    }
}

void Request::parseContent(std::string& buffer)
{
    (void)buffer;
    this->bodyEnd = 1;
}

void Request::printStatusBrowser(int clientFd)
{
    std::stringstream responseStream;
    responseStream << this->status.statusCode << " " << this->status.errorMsg;

    std::string response = responseStream.str();

    const char* responseData = response.c_str();
    int responseLength = response.length();
    write(clientFd, responseData, responseLength);
}