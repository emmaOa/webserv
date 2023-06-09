/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MainParsing.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouazzan <iouazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 08:57:24 by iouazzan          #+#    #+#             */
/*   Updated: 2023/06/09 17:35:28 by iouazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/parsing_file_cnf.hpp"

cnf *data_cnf = new cnf;


void initial_key_srv(void)
{
    data_cnf->m_s_key["port"] = 0;
    data_cnf->m_s_key["host"] = 0;
    data_cnf->m_s_key["server_name"] = 0;
    data_cnf->m_s_key["error_page_404"] = 0;
    data_cnf->m_s_key["error_page_400"] = 0;
    data_cnf->m_s_key["client_max_body_size"] = 0;
}

void initial_key_loc(void)
{
    data_cnf->m_l_key["root"] = 0;
    data_cnf->m_l_key["autoindex"] = 0;
    data_cnf->m_l_key["allow_methods"] = 0;
    data_cnf->m_l_key["index"] = 0;
    data_cnf->m_l_key["return"] = 0;
}

void initial_key_loc_cgi(void)
{
    data_cnf->m_l_c_key["root"] = 0;
    data_cnf->m_l_c_key["cgi_path"] = 0;
    data_cnf->m_l_c_key["cgi_ext"] = 0;
}

void init(void)
{
    initial_key_srv();
    initial_key_loc_cgi();
    initial_key_loc();
    data_cnf->br.first = 0;
    data_cnf->br.second = 0;
    data_cnf->if_map_2 = -1;
    data_cnf->is_cgi = false;
    data_cnf->location.first = 0;
    data_cnf->location.second = 0;
}

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
    if (data_cnf->is_cgi == false){
        if (data_cnf->m_l_key.count(word) > 0 && data_cnf->m_l_key[word] == 0)
            data_cnf->m_l_key[word] = 1;
        else {
            std::cout << word << "    invalid form : 10";
            return 1;
        }
    }
    else {
        if (data_cnf->m_l_c_key.count(word) > 0 && data_cnf->m_l_c_key[word] == 0)
            data_cnf->m_l_c_key[word] = 1;
        else {
            std::cout << "invalid form : 11";
            return 1;
        }
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
            if (word.compare("cgi-bin") == 0){
                data_cnf->location.second++;
                data_cnf->is_cgi = true;
            }
            else
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

int check_lct_value(std::string key)
{
    unsigned long i = 0;
    if (data_cnf->is_cgi == false){
        if (key.compare("autoindex") == 0){
            if (data_cnf->dq_2.size() != 1 || (data_cnf->dq_2[0] != "on" && data_cnf->dq_2[0] != "of")){
                std::cout << "invalid form 16\n";
                return 1;
            }   
        }
        else if (key.compare("allow_methods") == 0){
            while (i < data_cnf->dq_2.size())
            {
                if (data_cnf->dq_2[i] != "DELETE" && data_cnf->dq_2[i] != "POST" && data_cnf->dq_2[i] != "GET"){
                    std::cout << "invalid form 17\n";
                    return 1;  
                }
                i++;
            }
        }
        else if (key.compare("index") == 0){
            if (data_cnf->dq_2.size() != 1 || data_cnf->dq_2[0] != "index.html"){
                std::cout << "invalid form 18\n";
                return 1;
            }   
        }
        else {
            if (data_cnf->dq_2.size() != 1 || (access(data_cnf->dq_2[0].c_str(), F_OK) == -1) || (access(data_cnf->dq_2[0].c_str(), R_OK) == -1)){
                std::cout  << "invalid form 19\n";
                return 1;
            } 
        }
    }
    else {
        if (key.compare("root") == 0){
            if (data_cnf->dq_2.size() != 1 || (access(data_cnf->dq_2[0].c_str(), F_OK) == -1) || (access(data_cnf->dq_2[0].c_str(), R_OK) == -1)){
                std::cout  << "invalid form 20\n";
                return 1;
            }
        }
        else if (key.compare("cgi_path") == 0){
            while (i < data_cnf->dq_2.size())
            {
                if (access(data_cnf->dq_2[i].c_str(), F_OK) == -1 || access(data_cnf->dq_2[i].c_str(), R_OK) == -1){
                    std::cout  << "invalid form 21\n";
                    return 1;
                }
                i++;
            }
        }
        else {
            if (data_cnf->dq_2.size() < 1 ){
                std::cout  << "invalid form 22\n";
                return 1;
            }
        }
    }
    return 0;
}

int store_location(std::string line)
{
    std::string word;
    std::stringstream ss(line);
    std::string first;

    if (data_cnf->key_map1.empty()){
        if (key_location(line) != 0){
            std::cout << "invalid location header\n";
            return 1;
        }
    }
    else {
        while (ss >> word)
        {
            if (first.empty()){
                if (vld_location_key(word) != 0)
                    return 1;
                first = word;
            }
            else
                data_cnf->dq_2.push_back(word);
        }
        if (check_lct_value(first) != 0){
            data_cnf->dq_2.clear();
            return 1;
        }
        data_cnf->map_2.insert (std::pair<std::string, dq>(first, data_cnf->dq_2));
        data_cnf->dq_2.clear();
    }
    return 0;
}

int check_srv_value(std::string key)
{
    unsigned long i = 0;
    int port;
    int size;
    if (key.compare("port") == 0){
        while (i < data_cnf->dq_2.size())
        {
            port = strtod(data_cnf->dq_2[i].c_str(), NULL);
            if (port < 1 || port > 65535){
                std::cout << "invalid port\n";
                return 1;
            }
            i++;
        } 
    }
    else if (key.compare("host") == 0){
        if (data_cnf->dq_2.size() > 1){
            std::cout << "invalid forme 13\n";
            return 1;
        }
        if (data_cnf->dq_2[0].compare("127.0.0.1") != 0 && data_cnf->dq_2[0].compare("0.0.0.0") != 0 && \
             data_cnf->dq_2[0].compare("localhost") != 0 &&  !data_cnf->dq_2.empty()){
                        std::cout << "invalid forme 13\n";
            return 1;
        }
        else
            return 0;
    }
    else if (key.compare("server_name") == 0){
        if (data_cnf->dq_2.size() > 1){
            std::cout << "invalid forme 14\n";
            return 1;
        }
    }
    else if (key.compare("error_page_404") == 0){
        if (data_cnf->dq_2.size() > 1 || data_cnf->dq_2[0].compare("/error/404.html") != 0)
            data_cnf->dq_2[0] = "/error/404.html";
    }
    else if (key.compare("error_page_400") == 0){
        if (data_cnf->dq_2.size() > 1 || data_cnf->dq_2[0].compare("/error/400.html") != 0)
            data_cnf->dq_2[0] = "/error/400.html";
    }
    else if (key.compare("client_max_body_size") == 0){
       size = strtod(data_cnf->dq_2[0].c_str(), NULL);
       if (data_cnf->dq_2.size() > 1 || size < 0 || size > 1024){
            std::cout << "invalid forme 15\n";
            return 1;
       }
    }
    return 0;
}

int store(std::string line)
{
    std::string word;
    std::stringstream ss(line);
    mp_dq map_tmp; 
    std::string first;
    if (data_cnf->br.second == 0){
        while (ss >> word)
        {
            if (first.empty()){
                if (vld_srv_key(word) != 0)
                    return 1;
                first = word;
                // std::cout << word << std::endl;
            }
            else
                data_cnf->dq_2.push_back(word);
        }
        if (check_srv_value(first) != 0){
            data_cnf->dq_2.clear();
            return 1;
        }
        map_tmp.insert (std::pair<std::string, dq>("null", data_cnf->dq_2));
        data_cnf->map_1.insert (std::pair<std::string, mp_dq>(first, map_tmp));
        data_cnf->dq_2.clear();
        map_tmp.clear();
    }
    else{
        if (store_location(line) != 0)
            return 1;
    }
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

int ParcLine(std::string line)
{
    std::cout <<  line << std::endl;
    if (line.compare("server {") == 0 && data_cnf->br.second == 0){
        data_cnf->br.first++;
        return 0;
    }
    else if (data_cnf->br.first == 0 && (line.compare("server {") != 0)){
        std::cout << "invalid form : 01 : " << line << data_cnf->br.first<< std::endl;
        return 1;
    }
    else if (line.compare("}") == 0 || line.compare("    }") == 0){
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
        else{
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
    // while (ss >> word){

    //     std::cout << word ;
    // }
    // std::cout << '\n';

    // return 0;
}



int MainFileCnf(std::ifstream & CnfFile)
{
    std::string line;

    init();
    while ( getline (CnfFile,line) )
    {
        if (ParcLine(line) != 0)
            return 1;
    }
    CnfFile.close();
    if (data_cnf->br.first != 0 || data_cnf->br.second !=0){
        std::cout << "invalid form : 05\n";
        return 1;
    }
    return 0;
}
