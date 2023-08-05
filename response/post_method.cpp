/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_method.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nidor <nidor@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 14:18:55 by nidor             #+#    #+#             */
/*   Updated: 2023/08/05 15:12:09 by nidor            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

void postMethod(int sock_clt, int sock_srv)
{
    if (data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at(servs.at(sock_srv).clts.at(sock_clt).location).at("upload_is").at(0).compare("on") == 0)
    {
        std::string root;
		struct stat buf;

        root.assign(data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at(servs.at(sock_srv).clts.at(sock_clt).location).at("root").at(0));
        root.append("/toto");
        std::cout << "|root = " << root.c_str() << "|\n";
		// exit(0);
        
        std::cout << "|fd_mame ="  << servs.at(sock_srv).clts.at(sock_clt).fd_name.append("null") << "|\n";
		if (std::rename(servs.at(sock_srv).clts.at(sock_clt).fd_name.c_str(), root.c_str()) != 0)
            perror("Error renaming file");
        else
		    std::cout << "File renamed successfully";
    	// interruptResponse(sock_clt, sock_srv, "201", "Created");
    }
    else if (data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at(servs.at(sock_srv).clts.at(sock_clt).location).at("upload_is").at(0).compare("off") == 0)
    {
		// get(sock_clt, sock_srv); adapter avec post
    }
	close(sock_clt);
    servs.at(sock_srv).clts.erase(sock_clt);
}
