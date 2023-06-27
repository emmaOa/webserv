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

# include "./includes/webserv.hpp"
# include "./includes/parsing_file_cnf.hpp"
# include "./includes/socket.hpp"
#include "./includes/request.hpp"

int request_part(char *buffer,int lent, int sock_clt, int sock_srv)
{
    (void)lent;
    (void)sock_clt;
    (void)sock_srv;
    std::cout << buffer << std::endl;
    return 1;
}