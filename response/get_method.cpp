/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_method.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouazzan <iouazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 19:12:26 by namine            #+#    #+#             */
/*   Updated: 2023/07/28 04:51:07 by iouazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

void seve_error_file(int sock_clt, int sock_srv)
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
    
    response["Server: "] = data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at("server_name").at("null").at(0);
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
    // std::cout << "\n-------------------------------- RESPONSE HEADER : --------------------------------\n";
    // std::cout << header << "\n";
    // std::cout << std::rand() << std::endl;
    // std::cout << "-------------------------------------------------------------------------------------\n";
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
            seve_error_file(sock_clt, sock_srv);
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
    seve_error_file(sock_clt, sock_srv);
    close(sock_clt);
    servs.at(sock_srv).clts.erase(sock_clt);
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
        if (lstat(servs.at(sock_srv).clts.at(sock_clt).path.c_str(), &buf) == -1) {
            std::cout << "er = " << strerror(errno) << "\n";
            interruptResponse(sock_clt, sock_srv);
		    return (1); }
        else if(S_ISREG(buf.st_mode)) // file
        {
            std::cout << "lstat success.\n";
        	if (!data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at(servs.at(sock_srv).clts.at(sock_clt).location).at("cgi_is").at(0).compare("off"))
				file.open (servs.at(sock_srv).clts.at(sock_clt).path, std::ios::in | std::ios::binary | std::ios::ate);
			else {
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
        if (!send_header(sock_clt, sock_srv, size, servs.at(sock_srv).clts.at(sock_clt).path.c_str(), response))
            return (1);
        file.close();
        servs.at(sock_srv).clts.at(sock_clt).new_client++;
        return (0);
    }
    else
    {
        file.open (servs.at(sock_srv).clts.at(sock_clt).path.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
        if (!file.is_open()) { std::cout << "ayoo\n"; interruptResponse(sock_clt, sock_srv);
            return (1); }
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
        file.read (buffer, chunk); // protect read
        int temp = send(sock_clt, buffer, chunk, 0);
        if (temp != chunk)
        {
            std::cout << "SEND POSED\n";
            // exit(0);
            std::cout << temp << " * " << chunk << strerror(errno) <<std::endl;
            close(sock_clt);
            // it->second.clts.erase(w);
            servs.at(sock_srv).clts.erase(sock_clt);
            std::cout << "i av === " << i << "\n";
            // i--;
            std::cout << "i ap === " << i << "\n";
            // exit(0);
            delete[] buffer;
            return (1);
        }
        file.close();
        servs.at(sock_srv).clts.at(sock_clt).current_position += chunk;
        if (i)
        {
            i -= 1;
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
    // std::cout << "new sock_clt ============>>>> " << sock_clt << "\n";
    // print_request_header(sock_clt, sock_srv);
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
        if (getMethod(sock_clt, sock_srv, response)) {
            std::cout << "sock_clt = " << sock_clt << "\n";
            std::cout << "done\n";
            return (1);
        }
    }
    else if (servs.at(sock_srv).clts.at(sock_clt).request_map["method"] == "POST")
        postMethod(sock_clt, sock_srv);
    else
        deleteMethod(sock_clt, sock_srv);
    return (0);
    
   
    // std::cout << sock_clt << "sock_clt" << "\n";
    // std::cout << sock_srv << "sock_srv" << "\n";
    // std::cout << "err = |" << servs.at(sock_srv).clts.at(sock_clt).err << "|" << "\n";
    // std::cout << "err_msg = |" << servs.at(sock_srv).clts.at(sock_clt).err_msg << "|" << "\n";
    // (void)sock_srv;
    // char hello[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    // write(sock_clt , hello , sizeof(hello));
    // return 1;
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
