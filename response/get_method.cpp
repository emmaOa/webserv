/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_method.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emma <emma@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 14:18:46 by nidor             #+#    #+#             */
/*   Updated: 2023/08/07 01:08:51 by emma             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

typedef struct s_getVariables
{
    std::string cgiState;
    std::string cgiExtension;
    
    struct dirent *read_dir;
    std::ifstream file;
    long long int size;
    int chunk;
    char *buffer;
    int i;
    int rest;
    DIR *dir;

}t_getVariables;

void initializeVariables(int sock_clt, int sock_srv, t_getVariables *var)
{
    var->chunk = 1024;
    var->i = 0;
    if(data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at(servs.at(sock_srv).clts.at(sock_clt).location).at("cgi_is").at(0).compare("off") == 0)
        var->cgiState.assign("false");
    else
        var->cgiState.assign("true");
    if (servs.at(sock_srv).clts.at(sock_clt).is_ex_cgi == 0)
        var->cgiExtension.assign("false");
    else
        var->cgiExtension.assign("true");
}
std::string get_extension_type2(std::string type) {
    std::cout << "type : " << type << std::endl;
    std::vector<std::pair<std::string, std::string> > v;
    v.push_back(std::make_pair("text/css\r", ".css"));
    v.push_back(std::make_pair("text/csv\r", ".csv"));
    v.push_back(std::make_pair("image/gif\r", ".gif"));
    v.push_back(std::make_pair("text/html\r", ".html"));
    v.push_back(std::make_pair("image/x-icon\r", ".ico"));
    v.push_back(std::make_pair("image/jpeg\r", ".jpeg"));
    v.push_back(std::make_pair("application/javascript\r", ".js"));
    v.push_back(std::make_pair("application/json\r", ".json"));
    v.push_back(std::make_pair("image/png\r", ".png"));
    v.push_back(std::make_pair("application/pdf\r", ".pdf"));
    v.push_back(std::make_pair("text/plain\r", ".txt"));
    v.push_back(std::make_pair("image/svg+xml\r", ".svg"));
    v.push_back(std::make_pair("application/x-httpd-php\r", ".php"));

    for (unsigned long i = 0; i < v.size(); i++)
    {
        std::cout << "v : "<< v[i].first << std::endl;
        if (v[i].first.compare(type) == 0)
            return v[i].second;
    }
    return "null";
}

void getMethod(int sock_clt, int sock_srv)
{
    t_getVariables *var = new t_getVariables; // ne pas oublier de le delete !!!
    struct stat buf;
    std::vector<std::string> out;
    
    initializeVariables(sock_clt, sock_srv, var);
    if (servs.at(sock_srv).clts.at(sock_clt).new_client == 0)
    {
        servs.at(sock_srv).clts.at(sock_clt).path.assign(servs.at(sock_srv).clts.at(sock_clt).request_map["uri_new"]);
        if (lstat(servs.at(sock_srv).clts.at(sock_clt).path.c_str(), &buf) == -1)
            { interruptResponse(sock_clt, sock_srv, "404", "Not Found"); return ; }
        if(S_ISREG(buf.st_mode)) // file
        {
        	if (var->cgiState == "off" || (var->cgiState == "on" && var->cgiExtension == "false"))
            {
                var->file.open (servs.at(sock_srv).clts.at(sock_clt).path, std::ios::in | std::ios::binary | std::ios::ate);
            }
			else
            {
                f_cgi(sock_srv, sock_clt, servs.at(sock_srv).clts.at(sock_clt).path);
                 servs.at(sock_srv).clts.at(sock_clt).path.assign(servs.at(sock_srv).clts.at(sock_clt).file_cgi);
                if (servs.at(sock_srv).clts.at(sock_clt).type_cgi.compare("php") == 0)
                {
                    std::string str;
                    var->file.open (servs.at(sock_srv).clts.at(sock_clt).path, std::ios::in | std::ios::out);
                    std::fstream myfile;
                    myfile.open("example", std::ios::in | std::ios::out | std::ios::app);
                    while(getline(var->file, str))
                    {   
                        std::cout << str << "\n";
                        ft_split(str.c_str(), ':', out);
                        std::cout << "key = " << out[0] << "value = " << out[1] << "\n";
                        response[out[0]] = out[1];
                        out.clear();
                        if (str == "\r")
                        {
                            while (getline(var->file, str))
                            {
                                myfile << str;
                                myfile << "\n";
                            }
                        }
                    }
                    ft_split(response["Content-type"].c_str(), ';', out);
                    std::string ext;
                    if (out.size() == 2)
                        ext = get_extension_type2(out[0]);
                    else
                        ext = out[0];
                    out.clear();
                    servs.at(sock_srv).clts.at(sock_clt).path = servs.at(sock_srv).clts.at(sock_clt).path + ext;
                    rename("example", servs.at(sock_srv).clts.at(sock_clt).path.c_str());
                }
                else
                    var->file.open (servs.at(sock_srv).clts.at(sock_clt).path, std::ios::in | std::ios::binary | std::ios::ate);
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
                    var->file.open (servs.at(sock_srv).clts.at(sock_clt).path, std::ios::in | std::ios::binary | std::ios::ate);
                    if (!var->file)
                        { interruptResponse(sock_clt, sock_srv, "404", "Not Found"); return ;}
                }
                else if (index_exist && cgi_state == 1 && extension == 1)
                {
                    std::cout << "run cgi\n";
                    std::cout << "|" << servs.at(sock_srv).clts.at(sock_clt).path << "|\n";
                    f_cgi(sock_srv, sock_clt, servs.at(sock_srv).clts.at(sock_clt).path.append(index_name));
                    servs.at(sock_srv).clts.at(sock_clt).path.assign(servs.at(sock_srv).clts.at(sock_clt).file_cgi);
                    var->file.open (servs.at(sock_srv).clts.at(sock_clt).path, std::ios::in | std::ios::binary | std::ios::ate);
                    if (!var->file)
                        { interruptResponse(sock_clt, sock_srv, "404", "Not Found"); return ;}
                }
                else
                {
                    if (autoindex == 0)
                        { interruptResponse(sock_clt, sock_srv, "403", "Forbidden"); return ;} 
                    else
                    {
                        // get autoindex file placed in folder (should be "index.html")
                        std::string str;
                        std::cout << "path ===== |" << servs.at(sock_srv).clts.at(sock_clt).path << "|\n";
                        if ((var->dir = opendir (servs.at(sock_srv).clts.at(sock_clt).path.c_str())) != NULL)
                        {
                            str += "<html><head><title>Index of " + servs.at(sock_srv).clts.at(sock_clt).path + "</title></head><body bgcolor=\"white\"><h1>Index of "
                                    + servs.at(sock_srv).clts.at(sock_clt).path + "</h1><hr><ul><li><a href=\"../\">..</a></li>";

                            while ((var->read_dir = readdir (var->dir)) != NULL)
                            {
                                if (var->read_dir->d_name[0] != '.')
                                    str += "<li><a href=\"" + std::string(var->read_dir->d_name) + "\">" + std::string(var->read_dir->d_name) + "</a></li>";
                            }
                            str += "</ul><hr></body></html>";
                            closedir (var->dir);
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
        var->size = var->file.tellg();
        std::cout << "size = " << var->size << "\n";
        var->file.seekg (0, var->file.beg);
        var->rest = var->size % var->chunk;
        send_header(sock_clt, sock_srv, var->size, servs.at(sock_srv).clts.at(sock_clt).path.c_str()); // check if send header fail !
        var->file.close();
        servs.at(sock_srv).clts.at(sock_clt).new_client++;
        return ;
    }
    else
    {
        var->file.open (servs.at(sock_srv).clts.at(sock_clt).path, std::ios::in | std::ios::binary | std::ios::ate);
        if (!var->file.is_open())
            { interruptResponse(sock_clt, sock_srv, "404", "Not Found"); return ;}
        var->buffer = new char[1024];
        var->size = var->file.tellg();
        var->rest = var->size % var->chunk;
        if (servs.at(sock_srv).clts.at(sock_clt).current_position > var->size)
        {
            var->i = 1;
            servs.at(sock_srv).clts.at(sock_clt).current_position -= var->chunk;
            var->chunk = var->rest;
        }
        var->file.seekg (servs.at(sock_srv).clts.at(sock_clt).current_position, var->file.beg);
        var->file.read (var->buffer, var->chunk); // protect read
        if (send(sock_clt, var->buffer, var->chunk, 0) != var->chunk)
        {
            std::cout << "SEND POSED\n";
            var->file.close();
            close(sock_clt);
            servs.at(sock_srv).clts.erase(sock_clt);
            delete[] var->buffer;
            return ;
        }
        var->file.close();
        servs.at(sock_srv).clts.at(sock_clt).current_position += var->chunk;
        if (var->i)
        {
            var->file.close();
            close(sock_clt);
            servs.at(sock_srv).clts.erase(sock_clt);
            delete[] var->buffer;
            return ;
        }
    }
    delete[] var->buffer;
}
