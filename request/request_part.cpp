/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_part.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouazzan <iouazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 21:13:23 by iouazzan          #+#    #+#             */
/*   Updated: 2023/07/14 16:45:10 by iouazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

bool check_allowed_chars(std::string str)
{
    const std::string allowedChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:/";

    for (std::string::size_type i = 0; i < str.length(); ++i)
    {
        if (allowedChars.find(str[i]) == std::string::npos)
        {
            return 0;
        }
    }
    return 1;
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
    // int id_srv = port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host);
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
    match_location(sock_srv, sock_clt);
    // if (check_method(id_srv, servs.at(sock_srv).clts.at(sock_clt).request_map["uri_old"], servs.at(sock_srv).clts.at(sock_clt).request_map["method"]) < 1) {
    //     servs.at(sock_srv).clts.at(sock_clt).err = 405; 
    //     servs.at(sock_srv).clts.at(sock_clt).err_msg = " Method Not Allowed";
    // }
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

int he_to_in(std::string hex)
{
    int decimal;
    sscanf(hex.c_str(), "%x", &decimal);
    // std::cout << decimal << "\n";
    return decimal;
}

int request_part(char *buffer,int lent, int sock_clt, int sock_srv)
{
    (void)lent;

    std::cout << "request starte\n";
    if (servs.at(sock_srv).clts.at(sock_clt).is_done < 0) {
        std::fstream fd;
        std::fstream fd2;
        int i = 0;
        std::string name;
        std::string name2 = "test";
        std::string line;
        std::stringstream ss;
        int is_body = 0;
        ss << sock_clt;
        name = "file_" + ss.str() + random_String();
        fd.open(name.c_str(), std::ios::in | std::ios::out | std::ios::app);
        fd2.open(name2.c_str(),  std::ios::in | std::ios::out);
        if (!fd ) {
            std::cout << "Open failed" << std::endl;
            return -2;
        }
        if (!fd2) {
            std::cout << "Open failed2" << std::endl;
            return -2;
        }
        // std::cout << lent << " " << strlen(buffer) << "\n";
        fd2 << std::string(buffer, lent);
        fd2.seekp(0, std::ios::beg);
        // if (is_empty(fd2)){
        //     std::cout << "empty\n";
        // }
        while (std::getline(fd2, line))
        {
            // std::cout << i << std::endl;
            if (i == 0)
                first_line(line, sock_clt, sock_srv);
            else if ((line.find(':') != std::string::npos)){
                    pars_head(line, sock_clt, sock_srv);
            }
            if (!(line.find(':') != std::string::npos) && !(i == 0)) {
                if (is_body == 0)
                    is_body = 1;
            }
            if (is_body == 2)
                fd << line << std::endl;
            if (is_body == 1)
                is_body++;
            // line.replace(line.find(line),line.length(),"");
            i++;
        }
        check_err_head(sock_srv, sock_clt);
        // fallocate(fd, FALLOC_FL_COLLAPSE_RANGE, 0, 900);
        if (servs.at(sock_srv).clts.at(sock_clt).request_map["method"].compare("POST") == 0) {
            servs.at(sock_srv).clts.at(sock_clt).fd_name = name;
           servs.at(sock_srv).clts.at(sock_clt).is_done = 0;
        }
        else {
            servs.at(sock_srv).clts.at(sock_clt).is_done = 1;
            fd.close();
        }
        fd2.close();
        // std::cout << servs.at(sock_srv).clts.at(sock_clt).request_map["uri_new"] << "<<------\n";
    }
    else if (servs.at(sock_srv).clts.at(sock_clt).is_done == 0){
        std::fstream fd;
        fd.open(servs.at(sock_srv).clts.at(sock_clt).fd_name.c_str(), std::ios::in | std::ios::out | std::ios::app);
        if (!fd) {
            std::cout << "Open failed" << std::endl;
            return -2;
        }
        if (servs.at(sock_srv).clts.at(sock_clt).request_map.find("Transfer-Encoding") != servs.at(sock_srv).clts.at(sock_clt).request_map.end())
        {
            fd << std::string(buffer, lent);
            std::cout << lent << "\n";
            if (lent < 1024) {
                int len;
                int size = 0;
                std::string line;
                servs.at(sock_srv).clts.at(sock_clt).is_done = 1;
                fd.seekp(0, std::ios::beg);
                std::fstream fd2;
                fd2.open("ttt", std::ios::in | std::ios::out);
                if (!fd2) {
                    std::cout << "Open failed3" << std::endl;
                    return -2;
                }
                std::getline(fd, line);
                len = he_to_in(line);
                std::cout << "hexa : "<< line  << "\n";
                std::cout << "decimal : " << len  << "\n";
                while (std::getline(fd, line))
                { 
                    // while (size < len)
                    // {
                    // }
                    
                    size += line.length();
                    if (size > len){
                        
                        // size -= line.length();
                        // std::cout << "size : "<< size << "len : " << len << "\n";
                        // std::getline(fd, line);
                        // std::cout << line.length() << "\n";
                        // std::cout << line << "\n";
                        
                        len = he_to_in(line);
                        if (len == 0) {
                            std::cout << "hexa : "<< line  << "\n";
                            std::cout << "decimal : " << len  << "\n";
                            break;
                        }
                        std::cout << "hexa : "<< line  << "\n";
                        std::cout << "decimal : " << len  << "\n";
                        size = 0;
                        line.replace(line.begin(),line.end(), "");
                        // break;
                    }
                    else
                        fd2 << line << std::endl;
                }
            }
        }
        else {
            fd << std::string(buffer, lent);
            if (lent < 1024) 
                servs.at(sock_srv).clts.at(sock_clt).is_done = 1;
        }

    }
    return servs.at(sock_srv).clts.at(sock_clt).is_done;
}
