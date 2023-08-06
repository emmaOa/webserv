/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_method.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emma <emma@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 14:18:55 by nidor             #+#    #+#             */
/*   Updated: 2023/08/06 11:21:13 by emma             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"
std::string get_extension_type3(std::string type) {
    std::cout << "type : " << type << std::endl;
    std::vector<std::pair<std::string, std::string> > v;
    v.push_back(std::make_pair("text/css\r", ".css"));
    v.push_back(std::make_pair("text/csv\r", ".csv"));
    v.push_back(std::make_pair("image/gif\r", ".gif"));
    v.push_back(std::make_pair("text/html\r", ".html"));
    v.push_back(std::make_pair("image/x-icon\r", ".ico"));
    v.push_back(std::make_pair("image/jpeg\r", ".jpeg"));
    v.push_back(std::make_pair("application/javascript\r", ".js"));
    v.push_back(std::make_pair("application/json\r", ".json"));
    v.push_back(std::make_pair("image/png\r", ".png"));
    v.push_back(std::make_pair("application/pdf\r", ".pdf"));
    v.push_back(std::make_pair("text/plain\r", ".txt"));
    v.push_back(std::make_pair("image/svg+xml\r", ".svg"));
    v.push_back(std::make_pair("application/x-httpd-php\r", ".php"));


    for (unsigned long i = 0; i < v.size(); i++)
    {
        std::cout << "v : "<< v[i].first << std::endl;
        if (v[i].first.compare(type) == 0)
            return v[i].second;
    }
    return "null";
}

void postMethod(int sock_clt, int sock_srv)
{
    if (data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at(servs.at(sock_srv).clts.at(sock_clt).location).at("upload_is").at(0).compare("on") == 0)
    {
        std::string root;
		std::string ext;
        std::string name;
        std::string old;
        std::vector<std::string> out;
        if (servs.at(sock_srv).clts.at(sock_clt).type_cgi == "php" || servs.at(sock_srv).clts.at(sock_clt).type_cgi == "py") {
            ft_split(servs.at(sock_srv).clts.at(sock_clt).file_cgi, '/', out);
            name = out[out.size() - 1];
            out.clear();
            old = servs.at(sock_srv).clts.at(sock_clt).file_cgi;
        }
        else {
            ft_split(servs.at(sock_srv).clts.at(sock_clt).fd_name, '/', out);
            name = out[out.size() - 1];
            out.clear();
            old = servs.at(sock_srv).clts.at(sock_clt).fd_name;
            // name = servs.at(sock_srv).clts.at(sock_clt).fd_name;
        }
        
        if (servs.at(sock_srv).clts.at(sock_clt).request_map.find(" filename") != servs.at(sock_srv).clts.at(sock_clt).request_map.end()) 
            root = data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at(servs.at(sock_srv).clts.at(sock_clt).location).at("root").at(0) + "/" + servs.at(sock_srv).clts.at(sock_clt).request_map[" filename"];
        else {
            if (servs.at(sock_srv).clts.at(sock_clt).request_map.find("Content-type") != servs.at(sock_srv).clts.at(sock_clt).request_map.end())
                ext = get_extension_type3(servs.at(sock_srv).clts.at(sock_clt).request_map["Content-type"]);
            root = data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at(servs.at(sock_srv).clts.at(sock_clt).location).at("root").at(0) + "/" + name + "-upload" + ext;
        }
            
		if (std::rename(old.c_str(), root.c_str()) != 0)
            std::cout << "Error renaming file";
        // root.assign(data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at(servs.at(sock_srv).clts.at(sock_clt).location).at("root").at(0));
        // root.append("/toto");
        std::cout << "|root = " << root.c_str() << "|\n";
		// exit(0);
        
        // std::cout << "|fd_mame ="  << servs.at(sock_srv).clts.at(sock_clt).fd_name.append("null") << "|\n";
    	// interruptResponse(sock_clt, sock_srv, "201", "Created");
    }
    else if (data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at(servs.at(sock_srv).clts.at(sock_clt).location).at("upload_is").at(0).compare("off") == 0)
    {
		// get(sock_clt, sock_srv); adapter avec post
    }
	close(sock_clt);
    servs.at(sock_srv).clts.erase(sock_clt);
}
