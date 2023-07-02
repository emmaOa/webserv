/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MainParsing.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nidor <nidor@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 08:57:24 by iouazzan          #+#    #+#             */
/*   Updated: 2023/07/02 22:07:54 by nidor            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

cnf *data_cnf = new cnf;

int MainFileCnf(std::ifstream & CnfFile)
{
    std::string line;

    init();
    while ( getline (CnfFile,line) )
    {
        if (ParseLine(line) != 0)
            return 1;
    }
    CnfFile.close();
    if (data_cnf->br.first != 0 || data_cnf->br.second !=0){
        std::cout << "invalid form : 05\n";
        return 1;
    }
    return 0;
}

int ParseLine(std::string line)
{
    // std::cout <<  line << std::endl;
    if (line.compare("server {") == 0 && data_cnf->br.second == 0){
        data_cnf->br.first++;
        return 0;
    }
    else if (data_cnf->br.first == 0 && (line.compare("server {") != 0)){
        std::cout << "invalid form : 01 : " << line << data_cnf->br.first<< std::endl;
        return 1;
    }
    else if (line.compare("}") == 0 || line.compare("    }") == 0){
        if (close_brackets() != 0)
            return 1;
    }
    else if (is_location(line) == 0){
        if (data_cnf->br.first == 0){
            std::cout << "invalid form : 02\n";
            return 1;
        }
        data_cnf->if_map_2++;
        data_cnf->br.second++;
    }
    if (store(line) != 0)
        return 1;
    return 0;
}

int close_brackets(void)
{
    if (data_cnf->br.second > 0){
        if (data_cnf->is_cgi == false){
            if (all_key_location() != 0)
                return 1;
        }
        else {
            if ( all_key_location_cgi() != 0)
                return 1;
        }
        data_cnf->map_1.insert (std::pair<std::string, mp_dq>(data_cnf->key_map1, data_cnf->map_2));
        data_cnf->dq_2.clear();
        data_cnf->map_2.clear();
        data_cnf->br.second--;
        data_cnf->is_cgi = false;
        data_cnf->key_map1.clear();
    }
    else {
        if (all_key_srv() != 0 || data_cnf->location.first == 0 || data_cnf->location.second == 0){
            std::cout << "invalid form : 12\n";
            return 1;
        }
        data_cnf->servers.push_back(data_cnf->map_1);
        data_cnf->map_1.clear();
        data_cnf->br.first--;
        data_cnf->location.first = 0;
        data_cnf->location.second = 0;
    }
    if (data_cnf->br.first < 0 || data_cnf->br.second < 0){
        std::cout << "invalid form : 03\n";
        return 1;
    }
    return 0;
}
