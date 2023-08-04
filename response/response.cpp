/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nidor <nidor@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 19:59:15 by nidor             #+#    #+#             */
/*   Updated: 2023/08/03 23:00:10 by nidor            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

int response_part(int sock_clt, int sock_srv)
{
    print_request_header(sock_clt, sock_srv);
    
    if (exec_err == 500)
    {
        interruptResponse(sock_clt, sock_srv, "500", "Internal Server Error");
        return (1);
    }
    if (!servs.at(sock_srv).clts.at(sock_clt).new_client) {
        if (!proceedResponse(sock_clt, sock_srv))
        {
            close(sock_clt);
            servs.at(sock_srv).clts.erase(sock_clt);
            return (1);
        }
    }
    if (servs.at(sock_srv).clts.at(sock_clt).request_map["method"] == "GET")
    {
        if (getMethod(sock_clt, sock_srv))
            return (1);
    }
    if (servs.at(sock_srv).clts.at(sock_clt).request_map["method"] == "POST")
    {
        std::cout << "post called ... \n";
        if (postMethod(sock_clt, sock_srv))
            return (1);
    }
    if (servs.at(sock_srv).clts.at(sock_clt).request_map["method"] == "DELETE")
    {
        std::cout << "delete called ... \n";
        if (deleteMethod(sock_clt, sock_srv))
            return (1);
    }
    return (0);
}
