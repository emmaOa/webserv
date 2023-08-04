/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_method.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nidor <nidor@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 16:46:49 by namine            #+#    #+#             */
/*   Updated: 2023/08/04 01:07:38 by nidor            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

int postMethod(int sock_clt, int sock_srv)
{
    if (data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at(servs.at(sock_srv).clts.at(sock_clt).location).at("upload_is").at(0).compare("on") == 0)
    {
        std::string root;
		struct stat buf;

        root.assign(data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at(servs.at(sock_srv).clts.at(sock_clt).location).at("root").at(0));
        root.c_str();
        std::cout << "root " << root << "\n";
        exit(0);
        std::cout << "upload is on \n";
        
        std::cout << "||||||||||||" << servs.at(sock_srv).clts.at(sock_clt).fd_name.c_str() << "|||||||||||||\n";
        if (std::rename(servs.at(sock_srv).clts.at(sock_clt).fd_name.c_str(), "file.txt") != 0)
            perror("Error renaming file");
        else
		    std::cout << "File renamed successfully";
    	interruptResponse(sock_clt, sock_srv, "201", "Created");
    }
    else if (data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at(servs.at(sock_srv).clts.at(sock_clt).location).at("upload_is").at(0).compare("off") == 0)
    {
		// get(sock_clt, sock_srv); adapter avec post
    }
    return (1);
}
