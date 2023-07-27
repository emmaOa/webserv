/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_method.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namine <namine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 19:12:26 by namine            #+#    #+#             */
/*   Updated: 2023/07/27 18:28:37 by namine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

void serve_error_file(int sock_clt, int sock_srv)
{
    std::cout << "serve error file...\n";
    char hello[] = "HTTP/1.1 404 Not Found\nContent-Type: text/plain\nContent-Length: 107\n\nResource Not Found :( gonna add some html and css later with appropriate error message and status code ...!";
    write(sock_clt , hello , sizeof(hello));
    // servs.at(sock_srv).clts.at(sock_clt).err.assign("404");
    // servs.at(sock_srv).clts.at(sock_clt).err_msg.assign("Not Found");
}

int	send_header(int sock_clt, int sock_srv, int size, const char *path, std::map <std::string, std::string>& response)
{
    std::map<std::string,std::string>::iterator it;
    std::string header;

    response["ProtocolVersion"] = servs.at(sock_srv).clts.at(sock_clt).request_map["http_vr"].erase(8);
    response["StatusCode"] = servs.at(sock_srv).clts.at(sock_clt).err;
    response["ErrorMsg"] = servs.at(sock_srv).clts.at(sock_clt).err_msg;
    header.append(response["ProtocolVersion"]).append(" ");
    header.append(response["StatusCode"]).append(" ");
    header.append(response["ErrorMsg"]).append("\r\n");
    
    it = response.find("Location: ");
    if (it != response.end())
    {
        header.append("Location: ");
        header.append(response["Location: "]).append("\r\n\r\n");
        std::cout << "\n-------------------------------- RESPONSE WITH LOCATION HEADER : --------------------------------\n";
        std::cout << header << "\n";
        std::cout << "-------------------------------------------------------------------------------------\n";
        send(sock_clt, header.c_str(), header.length(), 0);
        return (0);
    }
    
    response["Server: "] = data_cnf->servers.at(0).at("server_name").at("null").at(0);
    header.append("Server: ");
    header.append(response["Server: "]).append("\r\n");
    
    header.append("Content-Type: ");
    response["Content-Type: "] = getContentType(path).append(";charset=UTF-8");
    header.append(response["Content-Type: "]).append("\r\n");
    
    header.append("Content-Length: ");
    response["Content-Length: "] = std::to_string(size);
    header.append(response["Content-Length: "]).append("\r\n\r\n");
    
    if (send(sock_clt, header.c_str(), header.length(), 0) < header.length())
    {
        std::cout << "SEND POSED IN HEADER !!!!\n";
        close(sock_clt);
        servs.at(sock_srv).clts.erase(sock_clt);
        return (0);
    }
    std::cout << "\n-------------------------------- RESPONSE HEADER : --------------------------------\n";
    std::cout << header << "\n";
    std::cout << "-------------------------------------------------------------------------------------\n";
    return (1);
}

int proceedResponse(int sock_clt, int sock_srv, std::map <std::string, std::string>& response)
{
    if (servs.at(sock_srv).clts.at(sock_clt).err.compare("null") != 0)
    {
        if (servs.at(sock_srv).clts.at(sock_clt).err.compare("301") == 0 || servs.at(sock_srv).clts.at(sock_clt).err.compare("302") == 0)
        {
            if (servs.at(sock_srv).clts.at(sock_clt).err.compare("301") == 0)
                servs.at(sock_srv).clts.at(sock_clt).err_msg = "Moved Permanently";
            if (servs.at(sock_srv).clts.at(sock_clt).err.compare("302") == 0)
                servs.at(sock_srv).clts.at(sock_clt).err_msg = "Found";
            response["Location: "] = servs.at(sock_srv).clts.at(sock_clt).path.append(servs.at(sock_srv).clts.at(sock_clt).request_map["uri_new"]);
            send_header(sock_clt, sock_srv, 0, servs.at(sock_srv).clts.at(sock_clt).path.c_str(), response);
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

void interruptResponse(int sock_clt, int sock_srv)
{
    serve_error_file(sock_clt, sock_srv);
    close(sock_clt);
    servs.at(sock_srv).clts.erase(sock_clt);
}

// int pathSecure(std::string path)
// {
//     std::cout << "path not secure\n";
//     return (path.find("..") == -1);
// }

int getMethod(int sock_clt, int sock_srv, std::map <std::string, std::string>& response)
{
    struct dirent *read_dir;
    long long int size;
    std::ifstream file;
    int chunk = 1024;
    struct stat buf;
    int i = 0;
    char *buffer;
    int rest;
    
    if (servs.at(sock_srv).clts.at(sock_clt).new_client == 0)
    {
        servs.at(sock_srv).clts.at(sock_clt).path.append(servs.at(sock_srv).clts.at(sock_clt).request_map["uri_new"]);
        if (lstat(servs.at(sock_srv).clts.at(sock_clt).path.c_str(), &buf) == -1)
            { interruptResponse(sock_clt, sock_srv); return (1); }
        else if(S_ISREG(buf.st_mode)) // file
        {
            std::cout << "lstat success.\n";
        	if (!data_cnf->servers.at(0).at(servs.at(sock_srv).clts.at(sock_clt).location).at("cgi_is").at(0).compare("off"))
				file.open (servs.at(sock_srv).clts.at(sock_clt).path, std::ios::in | std::ios::binary | std::ios::ate);
			else {
			    // run CGI
            }
        }
        else // dir
        {
			if (servs.at(sock_srv).clts.at(sock_clt).path.at(servs.at(sock_srv).clts.at(sock_clt).path.size() - 1) != '/')
			{
                std::cout << "gonna add slash\n";
                std::cout << "+++++++++++++++++++++++\n";
                servs.at(sock_srv).clts.at(sock_clt).path.append("/");
				servs.at(sock_srv).clts.at(sock_clt).err = "301";
				servs.at(sock_srv).clts.at(sock_clt).err_msg = "Moved Permanently";
                response["Location: "] = servs.at(sock_srv).clts.at(sock_clt).path.append(servs.at(sock_srv).clts.at(sock_clt).request_map["uri_new"]);
                send_header(sock_clt, sock_srv, 0, servs.at(sock_srv).clts.at(sock_clt).path.c_str(), response);
                std::cout << "+++++++++++++++++++++++\n";
                return (1);
			}
            else
            {
                // check if i have index in config file
                // if yes:
                    //  if cgi on -> Return Code Depend on cgi
                    // if cgi off ->return requested file
                    // if file has .py or .php extension and cgi off (mgs d'erreur)
                // if no 
                    // get autoindex file placed in root (should be "index.html")
                    // if (autoindex off) -> 403 Forbidden
                    // if (autoindex on) -> (list files)200 OK
            }
            DIR *dir = opendir(servs.at(sock_srv).clts.at(sock_clt).path.c_str());
            while ((read_dir = readdir(dir)) != NULL)
            {
                std::cout << read_dir->d_name << "\n";
                closedir(dir);
            }
        }
        size = file.tellg();
        file.seekg (0, file.beg);
        rest = size % chunk;
        if (!send_header(sock_clt, sock_srv, size, servs.at(sock_srv).clts.at(sock_clt).path.c_str(), response))
            return (1);
        file.close();
        servs.at(sock_srv).clts.at(sock_clt).new_client++;
        return (0);
    }
    else
    {
        file.open (servs.at(sock_srv).clts.at(sock_clt).path.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
        if (!file.is_open()) { interruptResponse(sock_clt, sock_srv);
            return (1); }
        buffer = new char[1024];
        size = file.tellg();
        rest = size % chunk;
        if (servs.at(sock_srv).clts.at(sock_clt).current_position > size)
        {
            i = 1;
            servs.at(sock_srv).clts.at(sock_clt).current_position -= chunk;
            chunk = rest;
        }
        file.seekg (servs.at(sock_srv).clts.at(sock_clt).current_position, file.beg);
        file.read (buffer, chunk); // protect read
        if (send(sock_clt, buffer, chunk, 0) != chunk)
        {
            std::cout << "SEND POSED\n";
            close(sock_clt);
            servs.at(sock_srv).clts.erase(sock_clt);
            delete[] buffer;
            return (1);
        }
        file.close();
        servs.at(sock_srv).clts.at(sock_clt).current_position += chunk;
        if (i)
        {
            close(sock_clt);
            servs.at(sock_srv).clts.erase(sock_clt);
            delete[] buffer;
            return (1);
        }
    }
    delete[] buffer;
    return (0);
}

int		response_part(int sock_clt, int sock_srv)
{
    std::map <std::string, std::string> response;
    
    print_request_header(sock_clt, sock_srv);
    if (!servs.at(sock_srv).clts.at(sock_clt).new_client) {
        if (!proceedResponse(sock_clt, sock_srv, response))
        {
            std::cout << "quit response.\n";
            close(sock_clt);
            servs.at(sock_srv).clts.erase(sock_clt);
            return (1);
        }
    }
    if (servs.at(sock_srv).clts.at(sock_clt).request_map["method"] == "GET")
    {
        if (getMethod(sock_clt, sock_srv, response))
            return (1);
    }
    if (servs.at(sock_srv).clts.at(sock_clt).request_map["method"] == "POST")
        postMethod(sock_clt, sock_srv);
    else
        deleteMethod(sock_clt, sock_srv);
    return (0);
}

// int response_part(int sock_clt, int sock_srv)
// {    
//     std::cout << sock_clt << "sock_clt" << "\n";
//     std::cout << sock_srv << "sock_srv" << "\n";
//     std::cout << "err = |" << servs.at(sock_srv).clts.at(sock_clt).err << "|" << "\n";
//     std::cout << "err_msg = |" << servs.at(sock_srv).clts.at(sock_clt).err_msg << "|" << "\n";
//     (void)sock_srv;
//     char hello[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
//     write(sock_clt , hello , sizeof(hello));
//     return 1;
// }

