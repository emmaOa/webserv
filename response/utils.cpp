/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namine <namine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 12:18:45 by namine            #+#    #+#             */
/*   Updated: 2023/07/30 05:21:14 by namine           ###   ########.fr       */
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

void		serve_error_file(int sock_clt, int sock_srv)
{
    std::string str;
    int index;
    
    str.assign("<!DOCTYPE html> <html lang=\"en\"> <head> <meta charset=\"UTF-8\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> <title> statusCode </title> <style> body { background-color: #f2f2f2; font-family: Arial, sans-serif; margin: 0; padding: 0; } .container { display: flex; flex-direction: column; align-items: center; justify-content: center; height: 100vh; text-align: center; } .error-code { font-size: 80px; font-weight: bold; margin-bottom: 20px; } .error-message { font-size: 24px; margin-bottom: 40px; } .home-link { color: #333; text-decoration: none; font-weight: bold; } .home-link:hover { text-decoration: underline; } </style> </head> <body> <div class=\"container\"> <h1 class=\"error-code\"> statusCode </h1> <p class=\"error-message\"> statusMessage </p> <p>Go back to <a class=\"home-link\" href=\"/\">homepage</a>.</p> </div> </body> </html>");
    
    index = str.find("statusCode");
    str.replace(index, 11, servs.at(sock_srv).clts.at(sock_clt).err);
    index = str.find("statusCode");
    str.replace(index, 11, servs.at(sock_srv).clts.at(sock_clt).err);
    index = str.find("statusMessage");
    str.replace(index, 14, servs.at(sock_srv).clts.at(sock_clt).err_msg);
    send_header(sock_clt, sock_srv, str.size(), ".html");
    send(sock_clt, str.c_str(), str.length(), 0);
}

int			pathSecure(std::string path)
{
    return (path.find(".."));
}

void		interruptResponse(int sock_clt, int sock_srv)
{
    serve_error_file(sock_clt, sock_srv);
    close(sock_clt);
    servs.at(sock_srv).clts.erase(sock_clt);
}
