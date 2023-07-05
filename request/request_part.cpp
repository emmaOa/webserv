/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_part.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouazzan <iouazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 21:13:23 by iouazzan          #+#    #+#             */
/*   Updated: 2023/06/26 21:17:13 by iouazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"
# include "../includes/parsing_file_cnf.hpp"
# include "../includes/socket.hpp"
#include "../includes/request.hpp"

std::string random_String()
{
    std::string randomString;
    const int length = 6;

    // Seed the random number generator
    std::srand(std::time(0));

    for (int i = 0; i < length; ++i)
    {
        // Generate a random ASCII character within the range of [a-z]
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
        servs.at(sock_srv).clts.at(sock_clt).fd_name = name;
        // std::cout << name << std::endl;
        if (!fd) {
            std::cout << "Open failed" << std::endl;
            return -2;
        }
        fd << buffer << std::endl;
        fd.seekp(0, std::ios::beg);
        if (is_empty(fd)){
            std::cout << "empty\n";
        }
        // std::cout << buffer << std::endl;
        while (std::getline(fd, line))
        {
            // std::cout << line << std::endl;
            if (i == 0)
                first_line(line, sock_clt, sock_srv);
            else {
                if ((line.find(':') != std::string::npos))
                    pars_head(line, sock_clt, sock_srv);
            }
            i++;
        }
        // std::cout << "====>" << servs.at(sock_srv).clts.at(sock_clt).request_map["Connection"] << "\n";
        fd.close();
        
    }

    // std::cout << buffer << std::endl;
    if (lent == 0)
        return 1;
    return 0;
}