/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vld_key.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouazzan <iouazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 17:53:48 by iouazzan          #+#    #+#             */
/*   Updated: 2023/07/25 02:13:55 by iouazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

int vld_srv_key(std::string word)
{
    if (word.compare("}") == 0)
        return 0;
    if (data_cnf->m_s_key.count(word) > 0 && data_cnf->m_s_key[word] == 0)
        data_cnf->m_s_key[word] = 1;
    else {
        std::cout << "invalid form : 06";
        return 1;
    }
    return 0;
}

int vld_location_key(std::string word)
{
    if (word.compare("}") == 0)
        return 0;
    if (data_cnf->m_l_key.count(word) > 0 && data_cnf->m_l_key[word] == 0)
        data_cnf->m_l_key[word] = 1;
    else {
        std::cout << word << "    invalid form : 10";
        return 1;
    }
    return 0;
}

int key_location(std::string line)
{
    std::string word;
    std::stringstream ss(line);
    int i = 0;

    while (ss >> word)
    {
        if (i == 0 && word.compare("location") != 0)
            return 1;
        else if (i == 0 && word.compare("location") == 0)
            i++;
        else if (i == 1){
            data_cnf->key_map1 = word;
            data_cnf->location.second++;
            data_cnf->location.first++;
            i++;
        }
        else if (i == 2 && word.compare("{") == 0)
            i++;
        else
            return 1;
    }
    if (i != 3){
    std::cout << data_cnf->key_map1  << i << "    :\n";
        return 1;

    }
    return 0;
}
