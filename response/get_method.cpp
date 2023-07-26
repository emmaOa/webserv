/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_method.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouazzan <iouazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 19:12:26 by namine            #+#    #+#             */
/*   Updated: 2023/07/26 02:40:39 by iouazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
    }
    return "application/octet-stream";
}

void	print_request_header(int sock_clt, int sock_srv)
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

void	send_header(int sock_clt, int sock_srv, int size, const char *path, std::map <std::string, std::string>& response)
{
    std::string header;
    std::map<std::string,std::string>::iterator it; 

    response["ProtocolVersion"] = servs.at(sock_srv).clts.at(sock_clt).request_map["http_vr"].erase(8);
    response["StatusCode"] = servs.at(sock_srv).clts.at(sock_clt).err;
    response["ErrorMsg"] = servs.at(sock_srv).clts.at(sock_clt).err_msg;
    header.append(response["ProtocolVersion"]).append(" ");
    header.append(response["StatusCode"]).append(" ");
    header.append(response["ErrorMsg"]).append("\n");
    it = response.find("Location: ");
    if (it != response.end())
    {
        std::cout << "Gonna add location to the response header ...\n";
        header.append("Location: ");
        header.append(response["Location: "]).append("\n");
    }
    response["Server: "] = data_cnf->servers.at(0).at("server_name").at("null").at(0);
    header.append("Server: ");
    header.append(response["Server: "]).append("\n");
    header.append("Content-Type: ");
    response["Content-Type: "] = getContentType(path).append(";charset=UTF-8");
    header.append(response["Content-Type: "]).append("\n");
    header.append("Content-Length: ");
    response["Content-Length: "] = std::to_string(size);
    header.append(response["Content-Length: "]).append("\n\n");
    send(sock_clt, header.c_str(), header.length(), 0);
    // std::cout << "\n-------------------------------- RESPONSE HEADER : --------------------------------\n";
    // std::cout << header << "\n";
    // std::cout << "-------------------------------------------------------------------------------------\n";
}

void seve_error_file(int sock_clt, int sock_srv)
{
    std::cout << "serve error file ...!";
}

void postMethod(int sock_clt, int sock_srv)
{
    // std::cout << "POST\n";
}

void deleteMethod(int sock_clt, int sock_srv)
{
    std::cout << "DELETE\n";
}

int proceedResponse(int sock_clt, int sock_srv, std::map <std::string, std::string>& response)
{
    if (servs.at(sock_srv).clts.at(sock_clt).err.compare("null") != 0)
    {
        if (servs.at(sock_srv).clts.at(sock_clt).err.compare("null") == 301) // 302 !!
        {
            response["Location: "] = servs.at(sock_srv).clts.at(sock_clt).path;
        }
        else
        {
            seve_error_file(sock_clt, sock_srv);
            return (0);
        }
    }
    return (1);
}

int getMethod(int sock_clt, int sock_srv, std::map <std::string, std::string>& response)
{
    struct dirent *read_dir;
    long long int size;
    std::ifstream file;
    int chunk = 1024;
    struct stat buf;
    static int i;
    char *buffer; 
    int rest;
    
    if (servs.at(sock_srv).clts.at(sock_clt).new_client == 0)
    {
        servs.at(sock_srv).clts.at(sock_clt).path.append(servs.at(sock_srv).clts.at(sock_clt).request_map["uri_new"]);
        lstat(servs.at(sock_srv).clts.at(sock_clt).path.c_str(), &buf);
        if(S_ISREG(buf.st_mode)) // file
        {
        	if (!data_cnf->servers.at(0).at(servs.at(sock_srv).clts.at(sock_clt).location).at("cgi_is").at(0).compare("off"))
			{
				file.open (servs.at(sock_srv).clts.at(sock_clt).path, std::ios::in | std::ios::binary | std::ios::ate);
				if (!file.is_open())
				{
					seve_error_file(sock_clt, sock_srv);
					close(sock_clt);
					return (1);
				}
			}
			else
			{
				// run CGI
			}
        }
        else // dir
        {
			// if (servs.at(sock_srv).clts.at(sock_clt).path.find("/", servs.at(sock_srv).clts.at(sock_clt).path.size()) == -1)
			// {
			// 	servs.at(sock_srv).clts.at(sock_clt).err = "301";
			// 	servs.at(sock_srv).clts.at(sock_clt).err_msg = "Moved Permanently";
            //     // add location
			// }
            // DIR *dir = opendir(servs.at(sock_srv).clts.at(sock_clt).path.c_str());
            // while ((read_dir = readdir(dir)) != NULL)
            // {
            //     std::cout << read_dir->d_name << "\n";
            //     closedir(dir);
            // }
        }
        size = file.tellg();
        file.seekg (0, file.beg);
        rest = size % chunk;
        send_header(sock_clt, sock_srv, size, servs.at(sock_srv).clts.at(sock_clt).path.c_str(), response);
        file.close();
        servs.at(sock_srv).clts.at(sock_clt).new_client++;
        return (0);
    }
    else
    {
        file.open (servs.at(sock_srv).clts.at(sock_clt).path.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
        if (!file.is_open())
        {
            seve_error_file(sock_clt, sock_srv);
            close(sock_clt);
            return (1);
        }
        // std::cout << "current_position = " << servs.at(sock_srv).clts.at(sock_clt).current_position << "\n";
        size = file.tellg();
        // std::cout << "size == " << size << "\n";
        rest = size % chunk;
        buffer = new char[1024];
        if (servs.at(sock_srv).clts.at(sock_clt).current_position > size)
        {
            i = 1;
            servs.at(sock_srv).clts.at(sock_clt).current_position -= chunk;
            chunk = rest;
        }
        file.seekg (servs.at(sock_srv).clts.at(sock_clt).current_position, file.beg);
        file.read (buffer, chunk);
        if (send(sock_clt, buffer, chunk, 0) < chunk)
        {
            i -= 1;
            close(sock_clt);
            delete[] buffer;
            return (1);
        } 
        file.close();
        servs.at(sock_srv).clts.at(sock_clt).current_position += chunk;
        if (i)
        {
            i -= 1;
            close(sock_clt);
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
    
    // print_request_header(sock_clt, sock_srv);
    if (!proceedResponse(sock_clt, sock_srv, response))
    {
        close(sock_clt);
        return (1);
    }
    if (servs.at(sock_srv).clts.at(sock_clt).request_map["method"] == "GET")
    {
        if (getMethod(sock_clt, sock_srv, response))
            return (1);
    }
    else if (servs.at(sock_srv).clts.at(sock_clt).request_map["method"] == "POST")
        postMethod(sock_clt, sock_srv);
    else
        deleteMethod(sock_clt, sock_srv);
    return (0);
}
