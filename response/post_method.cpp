# include "../includes/webserv.hpp"

void postMethod(int sock_clt, int sock_srv)
{
    if (data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at(servs.at(sock_srv).clts.at(sock_clt).location).at("upload_is").at(0).compare("on") == 0)
    {

        std::string root;
		std::string ext;
        std::string name;
        std::string old;
        std::vector<std::string> out;

        // check if its a file !!!!!
        ft_split(servs.at(sock_srv).clts.at(sock_clt).fd_name, '/', out);
        if (out.size() > 0)
            name = out[out.size() - 1];
        else
            name = out[0];
        std::cout << name << std::endl;
        out.clear();
        old = servs.at(sock_srv).clts.at(sock_clt).fd_name;

        if (servs.at(sock_srv).clts.at(sock_clt).request_map.find(" filename") != servs.at(sock_srv).clts.at(sock_clt).request_map.end()) 
            root = data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at(servs.at(sock_srv).clts.at(sock_clt).location).at("root").at(0) + "/" + servs.at(sock_srv).clts.at(sock_clt).request_map[" filename"];
        else {
            root = data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at(servs.at(sock_srv).clts.at(sock_clt).location).at("root").at(0) + "/" + name;
            std::cout << "|root = " << root.c_str() << "|\n";
        }
            
		if (std::rename(old.c_str(), root.c_str()) != 0) {

            std::cout << "|root = " << root.c_str() << "|\n" << "|old = " << old.c_str() << "|\n" ;
            std::cout << "rename faild \n";
            {interruptResponse(sock_clt, sock_srv, "500", "Internal Server Error"); return ;}
        }
    }
    interruptResponse(sock_clt, sock_srv, "201", "Created");
}
