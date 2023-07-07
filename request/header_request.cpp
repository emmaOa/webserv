/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header_request.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouazzan <iouazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 18:20:17 by iouazzan          #+#    #+#             */
/*   Updated: 2023/07/07 18:27:07 by iouazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

int nwe_path(std::string path, int sock_clt, int sock_srv)
{
    if ((path.find('?') != std::string::npos)) {
        std::vector<std::string> out; 
        const char delim = '?';
        split_one(path, delim, out);
        path = out[0];
        servs.at(sock_srv).clts.at(sock_clt).request_map["query"] = out[1];
    }
    return 0;
}

void split_one(std::string const &str, const char delim, std::vector<std::string> &out) 
{ 
    std::stringstream ss(str); 
 
    std::string s; 
    while (std::getline(ss, s, delim)) { 
        out.push_back(s); 
    } 
} 

int first_line(std::string line, int sock_clt, int sock_srv)
{
    std::vector<std::string> out; 
    const char delim = ' ';
    split_one(line, delim, out);
    // int i = 0;
    // while (i < out.size())
    // {
    //     std::cout << out[i];
    //     i++;
    // }
    std::pair<std::string, std::string>pr;
 
    servs.at(sock_srv).clts.at(sock_clt).request_map["method"] = out[0];
    servs.at(sock_srv).clts.at(sock_clt).request_map["uri_old"] = out[1];
    servs.at(sock_srv).clts.at(sock_clt).request_map["http_vr"] = out[2];

    return 0;
}

int pars_head(std::string line, int sock_clt, int sock_srv)
{
    std::vector<std::string> out; 
    const char delim = ':';

    split_one(line, delim, out);
    servs.at(sock_srv).clts.at(sock_clt).request_map[out[0]] = out[1];
    return 0;
}