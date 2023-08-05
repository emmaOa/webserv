/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_method.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nidor <nidor@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 14:18:46 by nidor             #+#    #+#             */
/*   Updated: 2023/08/05 14:18:47 by nidor            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

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

int getMethod(int sock_clt, int sock_srv)
{
    struct dirent *read_dir;
    std::ifstream file;
    long long int size;
    int chunk = 1024;
    struct stat buf;
    char *buffer;
    int i = 0;
    int rest;
    DIR *dir;
    
    if (servs.at(sock_srv).clts.at(sock_clt).new_client == 0)
    {
        servs.at(sock_srv).clts.at(sock_clt).path.assign(servs.at(sock_srv).clts.at(sock_clt).request_map["uri_new"]);
        // if (pathSecure(servs.at(sock_srv).clts.at(sock_clt).path) != -1)
        //     { interruptResponse(sock_clt, sock_srv, "403", "Forbidden"); return (1);}
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
                f_cgi(sock_srv, sock_clt, servs.at(sock_srv).clts.at(sock_clt).path);
                servs.at(sock_srv).clts.at(sock_clt).path.assign(servs.at(sock_srv).clts.at(sock_clt).file_cgi);
                if (servs.at(sock_srv).clts.at(sock_clt).type_cgi.compare("php") == 0)
                    file.open (servs.at(sock_srv).clts.at(sock_clt).path, std::ios::out);
                else
                    file.open (servs.at(sock_srv).clts.at(sock_clt).path, std::ios::in | std::ios::binary | std::ios::ate);
                if (file)
                {
                    std::cout << "opened\n";
                }
                // std::cout << "=======================================\n";
                // int         position;
                // std::string line;
                
                // while(getline(file, line, '\n'))
                // {
                //     position = line.find("\r");
                //     std::cout << "pos = " << position << "\n";
                //     if (position == -1)
                //     {
                //         line.replace(line.find(line), line.length(), "");
                //     }
                   
                //     // getline(file, line, '\n');
                //     // if (line.compare("\r") == 0)
                //     // {
                //     //     break ;
                //     //     // line.replace(line.find(line),line.length(),"");
                //     // }
                //     std::cout << line << "\n";
                // }
                // file.close();
                // std::cout << "=======================================\n";
            }
        }
        else // dir
        {
			if (servs.at(sock_srv).clts.at(sock_clt).path.at(servs.at(sock_srv).clts.at(sock_clt).path.size() - 1) != '/')
			{
                std::cout << "dir without / ... \n";
                servs.at(sock_srv).clts.at(sock_clt).path.assign(servs.at(sock_srv).clts.at(sock_clt).request_map["uri_old"]);
                servs.at(sock_srv).clts.at(sock_clt).path.append("/");
                std::cout <<  "path = " << servs.at(sock_srv).clts.at(sock_clt).path << "\n";
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
                std::cout << "dir with / ... \n";
                // initialisation ----------------------------------------------------------------------
                std::string index_name;
                index_name.assign(data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at(servs.at(sock_srv).clts.at(sock_clt).location).at("index").at(0));
                int cgi_state;
                if (data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at(servs.at(sock_srv).clts.at(sock_clt).location).at("cgi_is").at(0).compare("off") == 0)
                    cgi_state = 0;
                else
                    cgi_state = 1;
                int index_exist = data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at(servs.at(sock_srv).clts.at(sock_clt).location).at("index").size();
                int extension;
                if (check_ex_cgi_index(index_name, sock_clt, sock_srv).compare("null") == 0)
                    extension = 0; //false
                else
                    extension = 1; //true
                
                int autoindex;
                if (data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at(servs.at(sock_srv).clts.at(sock_clt).location).at("autoindex").at(0).compare("off") == 0)
                    autoindex = 0;
                else
                    autoindex = 1;
                std::cout << "|index_name = " << index_name << "|\n";
                std::cout << "|cgi_state = " << cgi_state << "|\n";
                std::cout << "|extention = " << extension << "|\n";
                std::cout << "|autoindex = " << autoindex << "|\n";
                // --------------------------------------------------------------------------------------
                if (index_exist && cgi_state == 0 && extension == 0) 
                {
                    servs.at(sock_srv).clts.at(sock_clt).path.append(index_name);
                    // std::cout << "|" << servs.at(sock_srv).clts.at(sock_clt).path << "|\n";
                    file.open (servs.at(sock_srv).clts.at(sock_clt).path, std::ios::in | std::ios::binary | std::ios::ate);
                    if (!file)
                        { interruptResponse(sock_clt, sock_srv, "404", "Not Found"); return (1);}
                }
                else if (index_exist && cgi_state == 1 && extension == 1)
                {
                    std::cout << "run cgi\n";
                    std::cout << "|" << servs.at(sock_srv).clts.at(sock_clt).path << "|\n";
                    f_cgi(sock_srv, sock_clt, servs.at(sock_srv).clts.at(sock_clt).path.append(index_name));
                    servs.at(sock_srv).clts.at(sock_clt).path.assign(servs.at(sock_srv).clts.at(sock_clt).file_cgi);
                    file.open (servs.at(sock_srv).clts.at(sock_clt).path, std::ios::in | std::ios::binary | std::ios::ate);
                    if (!file)
                        { interruptResponse(sock_clt, sock_srv, "404", "Not Found"); return (1);}
                }
                else
                {
                    if (autoindex = 0)
                        { interruptResponse(sock_clt, sock_srv, "403", "Forbidden"); return (1);} 
                    else
                    {
                        std::string str;
                        std::cout << "path ===== |" << servs.at(sock_srv).clts.at(sock_clt).path << "|\n";
                        // exit(0);
                        if ((dir = opendir (servs.at(sock_srv).clts.at(sock_clt).path.c_str())) != NULL)
                        {
                            str += "<html><head><title>Index of " + servs.at(sock_srv).clts.at(sock_clt).path + "</title></head><body bgcolor=\"white\"><h1>Index of "
                                    + servs.at(sock_srv).clts.at(sock_clt).path + "</h1><hr><ul><li><a href=\"../\">..</a></li>";

                            while ((read_dir = readdir (dir)) != NULL)
                            {
                                if (read_dir->d_name[0] != '.')
                                    str += "<li><a href=\"" + std::string(read_dir->d_name) + "\">" + std::string(read_dir->d_name) + "</a></li>";
                            }
                            str += "</ul><hr></body></html>";
                            closedir (dir);
                            send_header(sock_clt, sock_srv, str.length(), ".html");
                            send(sock_clt, str.c_str(), str.length(), 0);
                            close(sock_clt);
                            servs.at(sock_srv).clts.erase(sock_clt);
                            return (1);
                        }
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
            }
        }
        size = file.tellg();
        std::cout << "size = " << size << "\n";
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
