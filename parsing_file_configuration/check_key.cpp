/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_key.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nidor <nidor@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 17:51:32 by iouazzan          #+#    #+#             */
/*   Updated: 2023/07/02 19:28:33 by nidor            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

int all_key_srv(void)
{
    std::map<std::string, int>::iterator it = data_cnf->m_s_key.begin();
    while (it != data_cnf->m_s_key.end())
    {
        if (it->second != 1){
            std::cout << it->first << std::endl;
            std::cout << "invalid form : 07\n";
            return 1;
        }
        it++;
    }
    initial_key_srv();
    return 0;
}

int all_key_location(void)
{
    std::map<std::string, int>::iterator it = data_cnf->m_l_key.begin();
    while (it != data_cnf->m_l_key.end())
    {
        if (it->second != 1){
            std::cout << it->first << std::endl;
            std::cout << "invalid form : 08\n";
            return 1;
        }
        it++;
    }
    initial_key_loc();
    return 0;
}

int all_key_location_cgi(void)
{
    std::map<std::string, int>::iterator it = data_cnf->m_s_key.begin();
    while (it != data_cnf->m_s_key.end())
    {
        if (it->second != 1){
            std::cout << it->first << std::endl;
            std::cout << "invalid form : 09\n";
            return 1;
        }
        it++;
    }
    initial_key_loc_cgi();
    return 0;
}
