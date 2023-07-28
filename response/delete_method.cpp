/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_method.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namine <namine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 16:46:46 by namine            #+#    #+#             */
/*   Updated: 2023/07/28 23:04:32 by namine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

void deleteMethod(int sock_clt, int sock_srv, std::string path, std::string resource)
{
    if (resource.compare("file")) {
        std::cout << "gonna DELETE the file..\n";
        servs.at(sock_srv).clts.at(sock_clt).err.assign("204");
        servs.at(sock_srv).clts.at(sock_clt).err_msg.assign("No Content");
    }
    else
    {
        std::cout << "gonna DELETE the dir ..\n";
        servs.at(sock_srv).clts.at(sock_clt).err.assign("403");
        servs.at(sock_srv).clts.at(sock_clt).err_msg.assign("Forbidden");
    }
    if(remove(path.c_str()) != 0)
        std::cout << "Error deleting file\n";
    else
        std::cout << "File successfully deleted\n";
    interruptResponse(sock_clt, sock_srv);
}
