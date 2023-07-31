/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_method.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namine <namine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 18:28:46 by namine            #+#    #+#             */
/*   Updated: 2023/07/31 06:05:20 by namine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

std::map <std::string, std::string> response;

int send_header(int sock_clt, int sock_srv, int size, const char *path)
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
    
    response["Server: "] = data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at("server_name").at("null").at(0);
    header.append("Server: ");
    header.append(response["Server: "]).append("\r\n");
 
    header.append("Content-Type: ");
    response["Content-Type: "] = getContentType(path).append(";charset=UTF-8");
    header.append(response["Content-Type: "]).append("\r\n");
    
    header.append("Content-Length: ");
    response["Content-Length: "] = std::to_string(size);
    header.append(response["Content-Length: "]).append("\r\n\r\n");
    
    if (send(sock_clt, header.c_str(), header.length(), 0) < (ssize_t)header.length())
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

int proceedResponse(int sock_clt, int sock_srv)
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

int get_method(int sock_clt, int sock_srv)
{
    // struct dirent *read_dir;
    long long int size;
    std::ifstream file;
    int chunk = 1024;
    struct stat buf;
    int i = 0;
    char *buffer;
    int rest;
    
    if (servs.at(sock_srv).clts.at(sock_clt).new_client == 0)
    {
        servs.at(sock_srv).clts.at(sock_clt).path.assign(servs.at(sock_srv).clts.at(sock_clt).request_map["uri_new"]);
        if (pathSecure(servs.at(sock_srv).clts.at(sock_clt).path) != -1)
            { interruptResponse(sock_clt, sock_srv, "403", "Forbidden"); return (1);}
        if (lstat(servs.at(sock_srv).clts.at(sock_clt).path.c_str(), &buf) == -1)
            { interruptResponse(sock_clt, sock_srv, "404", "Not Found"); return (1); }
        if(S_ISREG(buf.st_mode)) // file
        {
        	if (data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at(servs.at(sock_srv).clts.at(sock_clt).location).at("cgi_is").at(0).compare("off") == 0 || (\
                data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at(servs.at(sock_srv).clts.at(sock_clt).location).at("cgi_is").at(0).compare("on") == 0 && \
                servs.at(sock_srv).clts.at(sock_clt).is_ex_cgi == 0))
                {
				    file.open (servs.at(sock_srv).clts.at(sock_clt).path, std::ios::in | std::ios::binary | std::ios::ate);
                }
			else
            {
                // std::string str;
                f_cgi(sock_srv, sock_clt);
                servs.at(sock_srv).clts.at(sock_clt).path.assign(servs.at(sock_srv).clts.at(sock_clt).file_cgi);
                if (servs.at(sock_srv).clts.at(sock_clt).type_cgi.compare("php") == 0)
                    file.open (servs.at(sock_srv).clts.at(sock_clt).path, std::ios::out);
                else
                    file.open (servs.at(sock_srv).clts.at(sock_clt).path, std::ios::in | std::ios::binary | std::ios::ate);
                if (file)
                {
                    std::cout << "opened\n";
                }
                std::cout << "=======================================\n";
                int         position;
                std::string line;
                
                while(getline(file, line, '\n'))
                {
                    position = line.find("\r");
                    std::cout << "pos = " << position << "\n";
                    if (position == -1)
                    {
                        line.replace(line.find(line), line.length(), "");
                    }
                   
                    // getline(file, line, '\n');
                    // if (line.compare("\r") == 0)
                    // {
                    //     break ;
                    //     // line.replace(line.find(line),line.length(),"");
                    // }
                    std::cout << line << "\n";
                }
                file.close();
                std::cout << "=======================================\n";
                exit (0);
            }
        }
        else // dir
        {
			if (servs.at(sock_srv).clts.at(sock_clt).path.at(servs.at(sock_srv).clts.at(sock_clt).path.size() - 1) != '/')
			{
                servs.at(sock_srv).clts.at(sock_clt).path.append("/");
				servs.at(sock_srv).clts.at(sock_clt).err = "301";
				servs.at(sock_srv).clts.at(sock_clt).err_msg = "Moved Permanently";
                response["Location: "].assign(servs.at(sock_srv).clts.at(sock_clt).path);
                send_header(sock_clt, sock_srv, 0, servs.at(sock_srv).clts.at(sock_clt).path.c_str());
                
                close(sock_clt);
                servs.at(sock_srv).clts.erase(sock_clt);
                return (1);
			}
            else
            {
                // check if i have index in config file
                // if yes:
                //      if cgi on -> Return Code Depend on cgi
                //     if cgi off ->return requested file
                //     if file has .py or .php extension and cgi off (mgs d'erreur)
                // if no 
                //     get autoindex file placed in root (should be "index.html")
                //     if (autoindex off) -> 403 Forbidden
                //     if (autoindex on) -> (list files)200 OK
                //     DIR *dir = opendir(servs.at(sock_srv).clts.at(sock_clt).path.c_str());
                //     while ((read_dir = readdir(dir)) != NULL)
                //     {
                //         std::cout << read_dir->d_name << "\n";
                //     }
                //         closedir(dir);
            }
        }
        size = file.tellg();
        file.seekg (0, file.beg);
        rest = size % chunk;
        if (!send_header(sock_clt, sock_srv, size, servs.at(sock_srv).clts.at(sock_clt).path.c_str()))
            return (1);
        file.close();
        servs.at(sock_srv).clts.at(sock_clt).new_client++;
        return (0);
    }
    else
    {
        file.open (servs.at(sock_srv).clts.at(sock_clt).path, std::ios::in | std::ios::binary | std::ios::ate);
        if (!file.is_open()) { interruptResponse(sock_clt, sock_srv, "404", "Not Found");
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

int response_part(int sock_clt, int sock_srv)
{
    print_request_header(sock_clt, sock_srv);
    
    if (exec_err == 500)
    {
        interruptResponse(sock_clt, sock_srv, "500", "Internal Server Error");
        return (1);
    }
    if (!servs.at(sock_srv).clts.at(sock_clt).new_client) {
        if (!proceedResponse(sock_clt, sock_srv))
        {
            close(sock_clt);
            servs.at(sock_srv).clts.erase(sock_clt);
            return (1);
        }
    }
    if (servs.at(sock_srv).clts.at(sock_clt).request_map["method"] == "GET")
    {
        if (get_method(sock_clt, sock_srv))
            return (1);
    }
    return (0);
}
