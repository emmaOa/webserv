/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_location.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouazzan <iouazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 18:20:28 by iouazzan          #+#    #+#             */
/*   Updated: 2023/07/08 21:28:00 by iouazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

int check_return(int sock_srv, int sock_clt, std::string location)
{
    int id_srv = port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host);

    if (data_cnf->servers.at(id_srv).at(location).at("return").size() == 1) {
        servs.at(sock_srv).clts.at(sock_clt).request_map["uri_new"] = data_cnf->servers.at(id_srv).at(location).at("return").at(0);
        std::cout << servs.at(sock_srv).clts.at(sock_clt).request_map["uri_new"] << "<===\n";
        return 1;
    }
    return 0;
}


int match_location(int sock_srv, int sock_clt)
{
    int id_srv = port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host); 
    std::string uri = servs.at(sock_srv).clts.at(sock_clt).request_map["uri_old"];
    // std::cout << "key  :  " << uri << "<===\n";
    m_mp_dq::iterator it;

    it = data_cnf->servers.at(id_srv).find(uri);
    if (it != data_cnf->servers.at(id_srv).end()) {
        std::cout << it->first << "<===\n";
        check_return(sock_srv, sock_clt, it->first);
        // servs.at(sock_srv).clts.at(sock_clt).request_map["uri_new"] = 
    }
    
    return 0;
}