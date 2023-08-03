/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_values.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namine <namine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 18:07:34 by iouazzan          #+#    #+#             */
/*   Updated: 2023/07/30 23:04:20 by namine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

int check_lct_value(std::string key)
{
    unsigned long i = 0;

    if (key.compare("autoindex") == 0){
        if (data_cnf->dq_2.size() != 1 || (data_cnf->dq_2[0] != "on" && data_cnf->dq_2[0] != "off")){
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
        if (data_cnf->dq_2.size() > 1){
            std::cout << "invalid form 18\n";
            return 1;
        }   
    }
    else if(key.compare("root") == 0){
        if (data_cnf->dq_2.size() != 1){
            std::cout  << data_cnf->dq_2[0] << "<----\n";
            std::cout  << "invalid form 200\n";
            return 1;
        } 
    }
    else  if(key.compare("return") == 0) {
        if ( data_cnf->dq_2.size() > 2 || \
            (data_cnf->dq_2.size() == 2 && data_cnf->dq_2[1].compare("301") != 0 && data_cnf->dq_2[1].compare("302"))){
            std::cout  << data_cnf->dq_2[0] << "<----\n";
            std::cout  << data_cnf->dq_2[1] << "<----\n";
            std::cout  << "invalid form 19\n";
            return 1;
        }
    }
    else  if(key.compare("upload_is") == 0) {
        if (data_cnf->dq_2.size() != 1 || (data_cnf->dq_2[0] != "on" && data_cnf->dq_2[0] != "off")) {
            std::cout  << "invalid form 19\n";
            return 1;
        }
    }
    else {
        if (data_cnf->dq_2.size() != 1 || (data_cnf->dq_2[0] != "on" && data_cnf->dq_2[0] != "off")){
            std::cout << "invalid form 25\n";
            return 1;
        }
    }
    return 0;
}

int check_host(std::string host)
{
    std::vector<std::string> out; 
    const char delim = '.';
    split_one(host, delim, out);
    if (out.size() != 4)
        return 1;
    for(unsigned long i = 0; i < out.size(); i++)
    {
        if (strtod(out[i].c_str(), NULL) < 0 || strtod(out[i].c_str(), NULL) > 255)
            return 1;
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
        if (data_cnf->dq_2.empty() || data_cnf->dq_2.size() > 1 || data_cnf->dq_2[0].compare("localhost") != 0){
            if (check_host(data_cnf->dq_2[0]) != 0) {
                std::cout << "invalid forme 13\n";
                return 1;
            }
        }
        else
            return 0;
    }
    else if (key.compare("server_name") == 0){
        if (data_cnf->dq_2.size() > 1){
            std::cout << "invalid forme 14\n";
            return 1;
        }
        else if(data_cnf->dq_2.size() == 0)
            data_cnf->dq_2.push_back("null");
    }
    else if (key.compare("404") == 0){
        if (data_cnf->dq_2.size() > 1 || data_cnf->dq_2[0].compare("/error/404.html") != 0)
            data_cnf->dq_2[0] = "./error/404.html";
    }
    else if (key.compare("400") == 0){
        if (data_cnf->dq_2.size() > 1 || data_cnf->dq_2[0].compare("/error/400.html") != 0)
            data_cnf->dq_2[0] = "./error/400.html";
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
