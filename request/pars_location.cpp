/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_location.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouazzan <iouazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 18:20:28 by iouazzan          #+#    #+#             */
/*   Updated: 2023/07/08 21:28:00 by iouazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

int check_return(int sock_srv, int sock_clt, std::string location)
{
    int id_srv = port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host);

    if (data_cnf->servers.at(id_srv).at(location).at("return").size() == 1) {
        servs.at(sock_srv).clts.at(sock_clt).request_map["uri_new"] = data_cnf->servers.at(id_srv).at(location).at("return").at(0);
        std::cout << servs.at(sock_srv).clts.at(sock_clt).request_map["uri_new"] << "<===\n";
        return 1;
    }
    return 0;
}

int match_location(int sock_srv, int sock_clt)
{
    int id_srv = port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host); 
    std::string uri = servs.at(sock_srv).clts.at(sock_clt).request_map["uri_old"];
    // std::cout << "key  :  " << uri << "<===\n";
    bool is_find = 0;
    std::vector<std::string> out; 
    const char delim = '/';
    split_one(uri, delim, out);
    unsigned int i = 0;

    if (out.size() > 1)
        out.erase(out.begin());
    
    // while (i < out.size())
    // {
    //     std::cout << out[i] << "---\n";
    //     i++;
    // }
    // std::cout << i << " : id\n";
    m_mp_dq::iterator it;
    unsigned int len = i;
    while (i > 0)
    {
        for (unsigned int l = 0; l < i; l++){
            if (l == 0)
                uri =  "/" + out[l];
            else
                uri = uri + "/" + out[l];
        }
        it = data_cnf->servers.at(id_srv).find(uri);
        if (it != data_cnf->servers.at(id_srv).end()) {
            // std::cout << it->first << "<===\n";
            servs.at(sock_srv).clts.at(sock_clt).location = it->first;
            if (check_return(sock_srv, sock_clt, it->first) == 0){
                uri = data_cnf->servers.at(id_srv).at(uri).at("root").at(0);
                while (i < len)
                {
                    uri += "/" + out[i];
                    i++;
                }
                servs.at(sock_srv).clts.at(sock_clt).request_map["uri_new"] = uri;
                // std::cout << uri << "<----\n";
            }
            is_find = 1;
            break;
        // servs.at(sock_srv).clts.at(sock_clt).request_map["uri_new"] = 

        }
        // std::cout  << i << "   uri :" << uri << "\n";
        i--;
    }
    // std::map<std::string, std::string>::iterator it2;
    // it2 = servs.at(sock_srv).clts.at(sock_clt).request_map.find("uri_new");
    if (is_find == 0){
        servs.at(sock_srv).clts.at(sock_clt).err = 404;
        servs.at(sock_srv).clts.at(sock_clt).err_msg = "Not found";
        // std::cout << "not found \n";
    }
    else {
        // if (check_method(id_srv, servs.at(sock_srv).clts.at(sock_clt).request_map["uri_new"], servs.at(sock_srv).clts.at(sock_clt).request_map["method"]) < 1) {
        //     servs.at(sock_srv).clts.at(sock_clt).err = 405; 
        //     servs.at(sock_srv).clts.at(sock_clt).err_msg = " Method Not Allowed";
        // }
        // // std::cout << id_srv
        if (find_method(data_cnf->servers.at(id_srv).at(servs.at(sock_srv).clts.at(sock_clt).location).at("allow_methods"), servs.at(sock_srv).clts.at(sock_clt).request_map["method"]) < 1) {
            servs.at(sock_srv).clts.at(sock_clt).err = 405; 
            servs.at(sock_srv).clts.at(sock_clt).err_msg = " Method Not Allowed";
            // std::cout << "not allowed \n";
        }
    }

    // it = data_cnf->servers.at(id_srv).find(uri);
    // if (it != data_cnf->servers.at(id_srv).end()) {
    //     std::cout << it->first << "<===\n";
    //     check_return(sock_srv, sock_clt, it->first);
    //     // servs.at(sock_srv).clts.at(sock_clt).request_map["uri_new"] = 
    // }

    return 0;
}

bool find_method(const std::deque<std::string>& methods, const std::string& target)
{
    for (std::deque<std::string>::const_iterator it = methods.begin(); it != methods.end(); ++it)
    {
        if (*it == target)
            return 1;
    }
    return 0;
}
