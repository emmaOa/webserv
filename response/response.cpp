# include "../includes/webserv.hpp"

void whichMethod(int sock_clt, int sock_srv, std::string method)
{
	if (method.compare("GET") == 0)
        getMethod(sock_clt, sock_srv);
    else if (method.compare("POST") == 0)
        postMethod(sock_clt, sock_srv);
    else if (method.compare("DELETE") == 0)
        deleteMethod(sock_clt, sock_srv);
}

void response_part(int sock_clt, int sock_srv)
{
	std::string method;
	
    print_request_header(sock_clt, sock_srv);
	method.assign(servs.at(sock_srv).clts.at(sock_clt).request_map["method"]);
    if (exec_err == 500)
    {
        interruptResponse(sock_clt, sock_srv, "500", "Internal Server Error");
        return ;
    }
    if (!servs.at(sock_srv).clts.at(sock_clt).new_client || servs.at(sock_srv).clts.at(sock_clt).err == "508")
	{
        if (!proceedResponse(sock_clt, sock_srv))
        {
            close(sock_clt);
            servs.at(sock_srv).clts.erase(sock_clt);
            return ;
        }
    }
	whichMethod(sock_clt, sock_srv, method);
}
