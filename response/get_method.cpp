/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_method.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouazzan <iouazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 19:13:22 by nidor             #+#    #+#             */
/*   Updated: 2023/07/18 23:45:32 by iouazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

int response_part(int sock_clt, int sock_srv)
{
    if (servs.at(sock_srv).clts.at(sock_clt).is_done == 1) {
        char hello[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nHello world!";
        write(sock_clt , hello , strlen(hello));
    }
    if (servs.at(sock_srv).clts.at(sock_clt).is_done == 1)
        servs.at(sock_srv).clts.at(sock_clt).is_done = 2;
    return 1;
}
