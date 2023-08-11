# include "../includes/webserv.hpp"

std::string	getContentType(const char* path)
{
    const char *last_dot = strrchr(path, '.');
    if (last_dot) {
        if (strcmp(last_dot, ".css") == 0) return "text/css";
        if (strcmp(last_dot, ".csv") == 0) return "text/csv";
        if (strcmp(last_dot, ".gif") == 0) return "image/gif";
        if (strcmp(last_dot, ".htm") == 0) return "text/html";
        if (strcmp(last_dot, ".html") == 0) return "text/html";
        if (strcmp(last_dot, ".ico") == 0) return "image/x-icon";
        if (strcmp(last_dot, ".jpeg") == 0) return "image/jpeg";
        if (strcmp(last_dot, ".jpg") == 0) return "image/jpeg";
        if (strcmp(last_dot, ".js") == 0) return "application/javascript";
        if (strcmp(last_dot, ".json") == 0) return "application/json";
        if (strcmp(last_dot, ".png") == 0) return "image/png";
        if (strcmp(last_dot, ".pdf") == 0) return "application/pdf";
        if (strcmp(last_dot, ".svg") == 0) return "image/svg+xml";
        if (strcmp(last_dot, ".txt") == 0) return "text/plain";
        if (strcmp(last_dot, ".mp4") == 0) return "video/mp4";
    }
    return "application/octet-stream";
}

void		print_request_header(int sock_clt, int sock_srv)
{
    std::cout << "\n-------------------------------- REQUEST HEADER : --------------------------------\n";
    std::cout << "sock_clt" << sock_clt << "\n";
    std::cout << "sock_srv" << sock_srv << "\n";
    std::map<std::string, std::string>::iterator it = servs.at(sock_srv).clts.at(sock_clt).request_map.begin();
    while (it != servs.at(sock_srv).clts.at(sock_clt).request_map.end())
    {
        std::cout << "Key=|" << it->first << "|=>|" << it->second << "|" << std::endl;
        ++it;
    }
    std::cout << "err = |" << servs.at(sock_srv).clts.at(sock_clt).err << "|" << "\n";
    std::cout << "err_msg = |" << servs.at(sock_srv).clts.at(sock_clt).err_msg << "|" << "\n";
    std::cout << "location = |" << servs.at(sock_srv).clts.at(sock_clt).location << "|" << "\n";
    std::cout << "-----------------------------------------------------------------------------------\n";
}

int			get_customized_error_file(int sock_clt, int sock_srv, const char *statusCode)
{
	char			*buffer;
	std::ifstream	file;
    long long int	size;
	
	file.open (data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at(statusCode).at("null").at(0).c_str(), std::ios::in | std::ios::binary | std::ios::ate);
	if (!file)
	{
		servs.at(sock_srv).clts.at(sock_clt).err.assign("404");
		servs.at(sock_srv).clts.at(sock_clt).err_msg.assign("Not Found");
		return (0);
	}
	else
	{
		size = file.tellg();
		buffer = new char[size];
		file.seekg (0, file.beg);
		file.read (buffer, size);
		send_header(sock_clt, sock_srv, size, ".html");
        if(send(sock_clt, buffer, size, 0) <= 0)
        {
            close(sock_clt);
            servs.at(sock_srv).clts.erase(sock_clt);
            return 0;
        }
        delete buffer;
	}
    file.close();
	return (1);
}

void		serve_error_file(int sock_clt, int sock_srv)
{
    std::string	str;

    if (servs.at(sock_srv).clts.at(sock_clt).err.compare("404") == 0)
	{
		if (get_customized_error_file(sock_clt, sock_srv, "404"))
			return ;
	}
	else if (servs.at(sock_srv).clts.at(sock_clt).err.compare("400") == 0)
	{
		if (get_customized_error_file(sock_clt, sock_srv, "400"))
			return ;
	}
    str.assign("<!DOCTYPE html> <html lang=\"en\"> <head> <meta charset=\"UTF-8\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> <title> statusCode </title> <style> body { background-color: #f2f2f2; font-family: Arial, sans-serif; margin: 0; padding: 0; } .container { display: flex; flex-direction: column; align-items: center; justify-content: center; height: 100vh; text-align: center; } .error-code { font-size: 80px; font-weight: bold; margin-bottom: 20px; } .error-message { font-size: 24px; margin-bottom: 40px; } .home-link { color: #333; text-decoration: none; font-weight: bold; } .home-link:hover { text-decoration: underline; } </style> </head> <body> <div class=\"container\"> <h1 class=\"error-code\"> statusCode </h1> <p class=\"error-message\"> statusMessage </p> <p>Go back to <a class=\"home-link\" href=\"/\">homepage</a>.</p> </div> </body> </html>");
    str.replace(str.find("statusCode"), 11, servs.at(sock_srv).clts.at(sock_clt).err);
    str.replace(str.find("statusCode"), 11, servs.at(sock_srv).clts.at(sock_clt).err);
    str.replace(str.find("statusMessage"), 14, servs.at(sock_srv).clts.at(sock_clt).err_msg);
    send_header(sock_clt, sock_srv, str.size(), ".html");
    if (send(sock_clt, str.c_str(), str.length(), 0) < (ssize_t)str.length())
    {
        std::cout << "SEND POSED IN HEADER 1\n";
        close(sock_clt);
        servs.at(sock_srv).clts.erase(sock_clt);
        return ;
    }
}

void		interruptResponse(int sock_clt, int sock_srv, const char *statusCode, const char *statusMessage)
{
	servs.at(sock_srv).clts.at(sock_clt).err.assign(statusCode);
	servs.at(sock_srv).clts.at(sock_clt).err_msg.assign(statusMessage);
    serve_error_file(sock_clt, sock_srv);
    close(sock_clt);
    servs.at(sock_srv).clts.erase(sock_clt);
}

int			proceedResponse(int sock_clt, int sock_srv)
{
    if (servs.at(sock_srv).clts.at(sock_clt).err.compare("null") != 0)
    {
        if (servs.at(sock_srv).clts.at(sock_clt).err.compare("301") == 0 || servs.at(sock_srv).clts.at(sock_clt).err.compare("302") == 0)
        {
            if (servs.at(sock_srv).clts.at(sock_clt).err.compare("301") == 0)
                servs.at(sock_srv).clts.at(sock_clt).err_msg = "Moved Permanently";
            if (servs.at(sock_srv).clts.at(sock_clt).err.compare("302") == 0)
                servs.at(sock_srv).clts.at(sock_clt).err_msg = "Found";
            servs.at(sock_srv).clts.at(sock_clt).path.assign(servs.at(sock_srv).clts.at(sock_clt).request_map["uri_new"]);
            response["Location: "] = servs.at(sock_srv).clts.at(sock_clt).path;
            send_header(sock_clt, sock_srv, 0, servs.at(sock_srv).clts.at(sock_clt).path.c_str());
            return (0);
        }
        else
        {
            serve_error_file(sock_clt, sock_srv);
            return (0);
        }
    }
    else if (servs.at(sock_srv).clts.at(sock_clt).err.compare("null") == 0)
    {
        servs.at(sock_srv).clts.at(sock_clt).err.assign("200");
        servs.at(sock_srv).clts.at(sock_clt).err_msg.assign("OK");
    }
    return (1);
}

int         send_header(int sock_clt, int sock_srv, long long int size, const char *path)
{
    std::map<std::string,std::string>::iterator it;
    std::string header;

    response["ProtocolVersion"] = servs.at(sock_srv).clts.at(sock_clt).request_map["http_vr"].erase(8);
    response["StatusCode"] = servs.at(sock_srv).clts.at(sock_clt).err;
    response["ErrorMsg"] = servs.at(sock_srv).clts.at(sock_clt).err_msg;
    header.append(response["ProtocolVersion"]).append(" ");
    header.append(response["StatusCode"]).append(" ");
    header.append(response["ErrorMsg"]).append("\r\n");
    
    if (servs.at(sock_srv).clts.at(sock_clt).err.compare("405") == 0)
    {
        header.append("Allow: ");
        int len = data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at(servs.at(sock_srv).clts.at(sock_clt).location).at("allow_methods").size();
        for(int i = 0; i < len; i++)
        {
            response["Allow"].append(data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at(servs.at(sock_srv).clts.at(sock_clt).location).at("allow_methods").at(i));
            if (i != len - 1)
                response["Allow"].append(", ");
        }
        header.append(response["Allow"]).append("\r\n");
    }

    it = response.find("Location: ");
    if (it != response.end())
    {
        header.append("Location: ");
        header.append(response["Location: "]).append("\r\n\r\n");
        std::cout << "\n-------------------------------- RESPONSE WITH LOCATION HEADER : --------------------------------\n";
        std::cout << header << "\n";
        std::cout << "-------------------------------------------------------------------------------------\n";
        if (send(sock_clt, header.c_str(), header.length(), 0) < (ssize_t)header.length()) // est ce que je close la connexion apres ??
        {
            std::cout << "SEND POSED IN HEADER 2\n";
            close(sock_clt);
            servs.at(sock_srv).clts.erase(sock_clt);
            return (0);
        }
        return (0);
    }

    it = response.find("Content-Type: ");
    if (it == response.end())
    {
        response["Content-Type: "] = getContentType(path);
    }
    header.append("Content-Type: ");
    header.append(response["Content-Type: "]).append("\r\n");
    
    header.append("Content-Length: ");
    response["Content-Length: "] = std::to_string(size);
    header.append(response["Content-Length: "]).append("\r\n\r\n");
    
    if (send(sock_clt, header.c_str(), header.length(), 0) < (ssize_t)header.length())
    {
        std::cout << "SEND POSED IN HEADER 2\n";
        close(sock_clt);
        servs.at(sock_srv).clts.erase(sock_clt);
        return (0);
    }
    response.clear();
    std::cout << "\n-------------------------------- RESPONSE HEADER : --------------------------------\n";
    std::cout << header << "\n";
    std::cout << "-------------------------------------------------------------------------------------\n";
    return (1);
}

void        ft_split(std::string const &str, const char delim, std::vector<std::string> &out) 
{ 
    std::stringstream ss(str); 
 
    std::string s; 
    while (std::getline(ss, s, delim)) { 
        out.push_back(s); 
    } 
}
