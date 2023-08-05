/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_method.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nidor <nidor@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 14:18:46 by nidor             #+#    #+#             */
/*   Updated: 2023/08/05 15:25:04 by nidor            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

void getMethod(int sock_clt, int sock_srv)
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
        if (lstat(servs.at(sock_srv).clts.at(sock_clt).path.c_str(), &buf) == -1)
            { interruptResponse(sock_clt, sock_srv, "404", "Not Found"); return ; }
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
                return ;
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
                        { interruptResponse(sock_clt, sock_srv, "404", "Not Found"); return ;}
                }
                else if (index_exist && cgi_state == 1 && extension == 1)
                {
                    std::cout << "run cgi\n";
                    std::cout << "|" << servs.at(sock_srv).clts.at(sock_clt).path << "|\n";
                    f_cgi(sock_srv, sock_clt, servs.at(sock_srv).clts.at(sock_clt).path.append(index_name));
                    servs.at(sock_srv).clts.at(sock_clt).path.assign(servs.at(sock_srv).clts.at(sock_clt).file_cgi);
                    file.open (servs.at(sock_srv).clts.at(sock_clt).path, std::ios::in | std::ios::binary | std::ios::ate);
                    if (!file)
                        { interruptResponse(sock_clt, sock_srv, "404", "Not Found"); return ;}
                }
                else
                {
                    if (autoindex = 0)
                        { interruptResponse(sock_clt, sock_srv, "403", "Forbidden"); return ;} 
                    else
                    {
                        // get autoindex file placed in folder (should be "index.html")
                        std::string str;
                        std::cout << "path ===== |" << servs.at(sock_srv).clts.at(sock_clt).path << "|\n";
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
                            return ;
                        }
                    }
                }
            }
        }
        size = file.tellg();
        std::cout << "size = " << size << "\n";
        file.seekg (0, file.beg);
        rest = size % chunk;
        send_header(sock_clt, sock_srv, size, servs.at(sock_srv).clts.at(sock_clt).path.c_str()); // check if send header fail !
        file.close();
        servs.at(sock_srv).clts.at(sock_clt).new_client++;
        return ;
    }
    else
    {
        file.open (servs.at(sock_srv).clts.at(sock_clt).path, std::ios::in | std::ios::binary | std::ios::ate);
        if (!file.is_open())
            { interruptResponse(sock_clt, sock_srv, "404", "Not Found"); return ;}
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
            file.close();
            close(sock_clt);
            servs.at(sock_srv).clts.erase(sock_clt);
            delete[] buffer;
            return ;
        }
        file.close();
        servs.at(sock_srv).clts.at(sock_clt).current_position += chunk;
        if (i)
        {
            file.close();
            close(sock_clt);
            servs.at(sock_srv).clts.erase(sock_clt);
            delete[] buffer;
            return ;
        }
    }
    delete[] buffer;
}
