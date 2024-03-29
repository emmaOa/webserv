/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_location.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emma <emma@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 18:20:28 by iouazzan          #+#    #+#             */
/*   Updated: 2023/08/06 06:35:12 by emma             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

int check_return(int sock_srv, int sock_clt, std::string location)
{
    int id_srv = port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host);

    if (data_cnf->servers.at(id_srv).at(location).at("return").size() > 0) {
        servs.at(sock_srv).clts.at(sock_clt).request_map["uri_new"] = data_cnf->servers.at(id_srv).at(location).at("return").at(0);
        // std::cout <<servs.at(sock_srv).clts.at(sock_clt).request_map["uri_new"] << "<===\n";
        if (data_cnf->servers.at(id_srv).at(location).at("return").size() == 2)
            servs.at(sock_srv).clts.at(sock_clt).err = data_cnf->servers.at(id_srv).at(location).at("return").at(1);
        else
            servs.at(sock_srv).clts.at(sock_clt).err = "302";
        return 1;
    }
    return 0;
}

int match_location(int sock_srv, int sock_clt)
{
    int s =0;
    int id_srv = port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host); 
    std::string uri = servs.at(sock_srv).clts.at(sock_clt).request_map["uri_old"];
    if (uri[uri.length() - 1] == '/')
        s =1;
    // std::cout << "key  :  " << uri << "<===\n";
    bool is_find = 0;
    std::vector<std::string> out; 
    const char delim = '/';
    split_one(uri, delim, out);
    
    if (out.size() > 1)
        out.erase(out.begin());
    unsigned int i = out.size();
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
            servs.at(sock_srv).clts.at(sock_clt).location = it->first;
            if (check_return(sock_srv, sock_clt, it->first) == 0){
                uri = data_cnf->servers.at(id_srv).at(uri).at("root").at(0);
                while (i < len)
                {
                    uri += "/" + out[i];
                    if (i == len - 1) {
                        if (servs.at(sock_srv).clts.at(sock_clt).request_map["method"].compare("GET") == 0)
                            servs.at(sock_srv).clts.at(sock_clt).fd_name = out[i];
                        check_ex_cgi(out[i], sock_srv, sock_clt);
                    }
                        // std::cout << "out uri is : "<< out[i] << std::endl;
                    i++;
                }
                servs.at(sock_srv).clts.at(sock_clt).request_map["uri_new"] = uri;
                if (s == 1)
                    servs.at(sock_srv).clts.at(sock_clt).request_map["uri_new"] =  servs.at(sock_srv).clts.at(sock_clt).request_map["uri_new"] + "/";
            }
            is_find = 1;
            break;
        }
        i--;
    }
    if (is_find == 0){
        servs.at(sock_srv).clts.at(sock_clt).err = "404";
        servs.at(sock_srv).clts.at(sock_clt).err_msg = "Not found";
    }
    else {
        if (find_method(data_cnf->servers.at(id_srv).at(servs.at(sock_srv).clts.at(sock_clt).location).at("allow_methods"), servs.at(sock_srv).clts.at(sock_clt).request_map["method"]) < 1) {
            servs.at(sock_srv).clts.at(sock_clt).err = "405"; 
            servs.at(sock_srv).clts.at(sock_clt).err_msg = "Method Not Allowed";
        }
    }
    if (servs.at(sock_srv).clts.at(sock_clt).location.empty())
        return 1;
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
