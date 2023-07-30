/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_method.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namine <namine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 16:46:49 by namine            #+#    #+#             */
/*   Updated: 2023/07/30 01:31:26 by namine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

void postMethod(int sock_clt, int sock_srv)
{
    (void)sock_clt;
    (void)sock_srv;
    std::cout << "POST :)\n";
}
