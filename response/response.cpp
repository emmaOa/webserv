/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nidor <nidor@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 14:20:48 by nidor             #+#    #+#             */
/*   Updated: 2023/08/05 14:20:52 by nidor            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

int whichMethod(int sock_clt, int sock_srv, std::string method)
{
	if (method.compare("GET") == 0)
    {
        if (getMethod(sock_clt, sock_srv))
            return (1);
    }
    else if (method.compare("POST") == 0)
    {
        if (postMethod(sock_clt, sock_srv))
            return (1);
    }
    else if (method.compare("DELETE") == 0)
    {
        if (deleteMethod(sock_clt, sock_srv))
            return (1);
    }
	return (0);
}

int response_part(int sock_clt, int sock_srv)
{
	std::string method;
	
    print_request_header(sock_clt, sock_srv);
	method.assign(servs.at(sock_srv).clts.at(sock_clt).request_map["method"]);
    if (exec_err == 500)
    {
        interruptResponse(sock_clt, sock_srv, "500", "Internal Server Error");
        return (1);
    }
    if (!servs.at(sock_srv).clts.at(sock_clt).new_client && (method.compare("GET") == 0 || method.compare("DELETE") == 0 ))
	{
        if (!proceedResponse(sock_clt, sock_srv))
        {
            close(sock_clt);
            servs.at(sock_srv).clts.erase(sock_clt);
            return (1);
        }
    }
	if (whichMethod(sock_clt, sock_srv, method))
		return (1);
    return (0);
}
