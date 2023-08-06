/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emma <emma@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 17:05:06 by iouazzan          #+#    #+#             */
/*   Updated: 2023/08/06 11:21:55 by emma             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

void initial_key_srv(void)
{
    data_cnf->m_s_key["port"] = 0;
    data_cnf->m_s_key["host"] = 0;
    data_cnf->m_s_key["server_name"] = 0;
    data_cnf->m_s_key["404"] = 0;
    data_cnf->m_s_key["400"] = 0;
    data_cnf->m_s_key["client_max_body_size"] = 0;
}


void initial_key_loc(void)
{
    data_cnf->m_l_key["root"] = 0;
    data_cnf->m_l_key["autoindex"] = 0;
    data_cnf->m_l_key["allow_methods"] = 0;
    data_cnf->m_l_key["index"] = 0;
    data_cnf->m_l_key["return"] = 0;
    data_cnf->m_l_key["cgi_is"] = 0;
    data_cnf->m_l_key["upload_is"] = 0;
}

void init(void)
{
    initial_key_srv();
    initial_key_loc();
    data_cnf->br.first = 0;
    data_cnf->br.second = 0;
    data_cnf->if_map_2 = -1;
    data_cnf->location.first = 0;
    data_cnf->location.second = 0;
}
