/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_method.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nidor <nidor@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 19:13:22 by nidor             #+#    #+#             */
/*   Updated: 2023/07/08 01:54:59 by nidor            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

int response_part(int sock_clt, int sock_srv)
{
    std::map<std::string, std::string>::iterator it = servs.at(sock_srv).clts.at(sock_clt).request_map.begin();
    
    std::cout << "------------------------------------------\n";
    while (it != servs.at(sock_srv).clts.at(sock_clt).request_map.end())
    {
        std::cout << "Key: " << it->first << ", Value: " << it->second << "\n";
        ++it;
    }
    std::cout << "------------------------------------------\n";
    (void)sock_srv;
    char hello[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    write(sock_clt , hello , sizeof(hello));
    return (1);
}
