/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MainParsing.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouazzan <iouazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 08:57:24 by iouazzan          #+#    #+#             */
/*   Updated: 2023/05/27 11:23:09 by iouazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/parsing_file_cnf.hpp"
cnf *data_cnf = new cnf;

int stor_location(std::string line)
{
    std::string word;
    std::stringstream ss(line);
    std::string first;

    if (data_cnf->if_map_2 == 0)
        data_cnf->key_map1 = line;
    else {
        while (ss >> word)
        {
            if (first.empty())
                first = word;
            else
                data_cnf->dq_2.push_back(word);
        }
        data_cnf->map_2.insert (std::pair<std::string, dq>(first, data_cnf->dq_2));
    }

    return 0;
}

int stor(std::string line)
{
    std::string word;
    std::stringstream ss(line);
    mp_dq map_tmp; 
    std::string first;
    if (data_cnf->br.second < 1){
        while (ss >> word)
        {
            if (first.empty())
                first = word;
            else
                data_cnf->dq_2.push_back(word);
        }
        map_tmp.insert (std::pair<std::string, dq>("null", data_cnf->dq_2));
        data_cnf->map_1.insert (std::pair<std::string, mp_dq>(first, map_tmp));
        data_cnf->dq_2.clear();
        map_tmp.clear();
    }
    else
        stor_location(line);
    return 0;
}

int is_location(std::string line)
{
    std::string word;
    int equal = 0;
    std::stringstream ss(line);
    while (ss >> word){
        if (equal == 0 && word.compare("location") == 0){
            equal++;
        }
        else if (equal == 1 && word.compare("{") == 0 && !(ss >> word)){
            equal++;
            
        }
        else if (equal == 1 && word.compare("}") == 0){
            std::cout << "invalid form : 04\n";
            return 1;
        }
    }
    if (equal == 2)
        return 0;
    return 1;
}

int ParcLine(std::string line)
{
    if (data_cnf->br.first == 0 && (line.compare("server {") == 0 && data_cnf->br.second == 0)){
        data_cnf->br.first++;
        return 0;
    }
    else if (data_cnf->br.first == 0 && (line.compare("server {") != 0)){
        std::cout << "invalid form : 01\n";
        return 1;
    }
    else if (line.compare("}") == 0){
        if (data_cnf->br.second > 0){
            data_cnf->br.second--;
        }
        data_cnf->br.first--;
        if (data_cnf->br.first < 0 || data_cnf->br.second < 0){
            std::cout << "invalid form : 03\n";
            return 1;
        }
        if (data_cnf->br.second == 0 && !data_cnf->map_2.empty()){
            data_cnf->map_1.insert (std::pair<std::string, mp_dq>(data_cnf->key_map1, data_cnf->map_2));
            data_cnf->dq_2.clear();
            data_cnf->map_2.clear();
        }
        else if (data_cnf->br.first == 0 && !data_cnf->map_1.empty()){
            data_cnf->servers.push_back(data_cnf->map_1);
            data_cnf->map_1.clear();
        }
    }
    else if (is_location(line) == 0){
        if (data_cnf->br.first == 0){
            std::cout << "invalid form : 02\n";
            return 1;
        }
        data_cnf->if_map_2++;
        data_cnf->br.second++;
    }

    if (stor(line) != 0)
        return 1;

    return 0;
    // while (ss >> word){

    //     std::cout << word ;
    // }
    // std::cout << '\n';

    // return 0;
}

int MainFileCnf(void)
{
    std::ifstream CnfFile;
    std::string line;
    
    CnfFile.open("parsing_file_configuration/configuration_file.txt");
    if (CnfFile.is_open()){
        data_cnf->br.first = 0;
        data_cnf->br.second = 0;
        data_cnf->if_map_2 = -1;
        while ( getline (CnfFile,line) )
        {
            if (ParcLine(line) != 0)
                return 1;
        }
        CnfFile.close();
    }
    else
        std::cout << "open field";
    return 0;
}
