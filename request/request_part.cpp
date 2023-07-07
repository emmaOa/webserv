/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_part.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouazzan <iouazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 21:13:23 by iouazzan          #+#    #+#             */
/*   Updated: 2023/07/07 19:27:15 by iouazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

bool check_allowed_chars(std::string str)
{
    const std::string allowedChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    for (std::string::size_type i = 0; i < str.length(); ++i)
    {
        if (allowedChars.find(str[i]) == std::string::npos)
        {
            return 0;
        }
    }
    return 1;
}

int check_method(int id, std::string location, std::string method)
{
    std::deque<std::string>::iterator it;

    // std::cout << "id : " << id << " - location : " << location << " - method : " << method << "\n";
    for (unsigned int i = 0; i < data_cnf->servers.at(id).at(location).at("allow_methods").size(); i++){
        if (data_cnf->servers.at(id).at(location).at("allow_methods").at(i).compare(method) == 0)
            return 1;  
    }
    return 0;
}

int port_srv(int port, std::string host)
{
    // std::cout << "port : " << port << "host :"<< host << "\n";
    int i = 0;
    int id = -1;
    std::deque <m_mp_dq >::iterator it;
    for (it = data_cnf->servers.begin(); it != data_cnf->servers.end(); it++){
        if (strtod(data_cnf->servers.at(i).at("port").at("null").at(0).c_str(), NULL) == port && \
                data_cnf->servers.at(i).at("host").at("null").at(0).compare(host) == 0) 
            id = i;
        i++;
    }
    return id;
}

void check_err_head(int sock_srv, int sock_clt)
{
    int id_srv = port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host);
    if (servs.at(sock_srv).clts.at(sock_clt).request_map.find("Transfer-Encoding") != servs.at(sock_srv).clts.at(sock_clt).request_map.end()) {
        if (servs.at(sock_srv).clts.at(sock_clt).request_map["Transfer-Encoding"] != "chunked") {
            servs.at(sock_srv).clts.at(sock_clt).err = 501;
            servs.at(sock_srv).clts.at(sock_clt).err_msg = "Not Implemented";
        }
    }
    if (servs.at(sock_srv).clts.at(sock_clt).request_map.find("Transfer-Encoding") == servs.at(sock_srv).clts.at(sock_clt).request_map.end() && servs.at(sock_srv).clts.at(sock_clt).request_map.find("Content-Length") == servs.at(sock_srv).clts.at(sock_clt).request_map.end()) {
        servs.at(sock_srv).clts.at(sock_clt).err = 400;
        servs.at(sock_srv).clts.at(sock_clt).err_msg = "Bad Request";
    }
    if (servs.at(sock_srv).clts.at(sock_clt).request_map["uri_old"].length() > 2048) {
        servs.at(sock_srv).clts.at(sock_clt).err = 414;
        servs.at(sock_srv).clts.at(sock_clt).err_msg = "Request-URI Too Long";
    }
    if (check_allowed_chars(servs.at(sock_srv).clts.at(sock_clt).request_map["uri_old"]) < 1) {
        servs.at(sock_srv).clts.at(sock_clt).err = 400;
        servs.at(sock_srv).clts.at(sock_clt).err_msg = "Bad Request";
    }
    if (check_method(id_srv, servs.at(sock_srv).clts.at(sock_clt).request_map["uri_old"], servs.at(sock_srv).clts.at(sock_clt).request_map["method"]) < 1) {
        servs.at(sock_srv).clts.at(sock_clt).err = 405; 
        servs.at(sock_srv).clts.at(sock_clt).err_msg = " Method Not Allowed";
    }
}

std::string random_String()
{
    std::string randomString;
    const int length = 6;

    std::srand(std::time(0));

    for (int i = 0; i < length; ++i)
    {
        char randomChar = 'a' + (std::rand() % 26);
        randomString.push_back(randomChar);
    }

    return randomString;
}

bool is_empty(std::fstream& pFile)
{
    return pFile.peek() == std::ifstream::traits_type::eof();
}

int request_part(char *buffer,int lent, int sock_clt, int sock_srv)
{
    (void)lent;

    std::cout << "request starte\n";
    if (servs.at(sock_srv).clts.at(sock_clt).fd_name.compare("null") == 0) {
        std::fstream fd;
        int i = 0;
        std::string name;
        std::string line;
        std::string buf(buffer);
        std::stringstream ss;
        ss << sock_clt;
        name = "file_" + ss.str() + random_String();
        fd.open(name.c_str(), std::ios::in | std::ios::out | std::ios::app);
        if (!fd) {
            std::cout << "Open failed" << std::endl;
            return -2;
        }
        fd << buffer << std::endl;
        fd.seekp(0, std::ios::beg);
        if (is_empty(fd)){
            std::cout << "empty\n";
        }
        while (std::getline(fd, line))
        {
            if (i == 0)
                first_line(line, sock_clt, sock_srv);
            else {
                if ((line.find(':') != std::string::npos))
                    pars_head(line, sock_clt, sock_srv);
            }
            i++;
        }
        check_err_head(sock_srv, sock_clt);
        if (servs.at(sock_srv).clts.at(sock_clt).request_map["method"].compare("GET") == 0 || \
            servs.at(sock_srv).clts.at(sock_clt).request_map["method"].compare("DELETE") == 0){
                servs.at(sock_srv).clts.at(sock_clt).fd_name = "done";
                
            }
        else
            servs.at(sock_srv).clts.at(sock_clt).fd_name = name;
        fd.close();
        return 1;
    }
    else if (servs.at(sock_srv).clts.at(sock_clt).fd_name.compare("done") == 0)
        return 1;
    else
        return 0;
}