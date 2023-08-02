/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_method.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nidor <nidor@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 16:46:49 by namine            #+#    #+#             */
/*   Updated: 2023/08/02 20:41:37 by nidor            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

int postMethod(int sock_clt, int sock_srv)
{
    (void)sock_clt;
    (void)sock_srv;
    std::cout << "post methode \n";
    return (1);
    // interruptResponse(sock_clt, sock_srv, "201", "Created");
}
