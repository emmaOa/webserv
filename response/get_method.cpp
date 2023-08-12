# include "../includes/webserv.hpp"

typedef struct s_getVariables
{
    int				indexInConfigFile;
    std::string		indexExtension;
    std::string		cgiExtension;
    std::string		indexName;
    std::string		autoindex;
    struct dirent	*read_dir;
    std::string		cgiState;
    char			*buffer;
    int				chunk;
    std::ifstream	file;
    long long int	size;
    int				rest;
    DIR				*dir;
    std::string     finish;
}t_getVariables;

void    initializeVariables(int sock_clt, int sock_srv, t_getVariables *var)
{
    var->chunk = 1024;
    if(data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at(servs.at(sock_srv).clts.at(sock_clt).location).at("cgi_is").at(0).compare("off") == 0)
        var->cgiState.assign("off");
    else
        var->cgiState.assign("on");
  
    if (servs.at(sock_srv).clts.at(sock_clt).is_ex_cgi == 0)
        var->cgiExtension.assign("false");
    else
        var->cgiExtension.assign("true");
 
    var->indexInConfigFile = data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at(servs.at(sock_srv).clts.at(sock_clt).location).at("index").size();
    if (var->indexInConfigFile)
        var->indexName.assign(data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at(servs.at(sock_srv).clts.at(sock_clt).location).at("index").at(0));

    if (data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at(servs.at(sock_srv).clts.at(sock_clt).location).at("autoindex").at(0).compare("off") == 0)
        var->autoindex.assign("off");
    else
        var->autoindex.assign("on");

    if (check_ex_cgi_index(var->indexName, sock_clt, sock_srv).compare("null") == 0)
        var->indexExtension = "false";
    else
        var->indexExtension = "true";
}

void    sendResponse(int sock_clt, int sock_srv, t_getVariables *var)
{
    std::cout << "gonna serve body \n";
    initializeVariables(sock_clt, sock_srv, var);
    var->file.open (servs.at(sock_srv).clts.at(sock_clt).path.c_str(), std::ios::binary);
    if (!var->file.is_open())
        { interruptResponse(sock_clt, sock_srv, "404", "Not Found"); delete var; return ;}
    if(servs.at(sock_srv).clts.at(sock_clt).current_position + var->chunk > servs.at(sock_srv).clts.at(sock_clt).sizeOfReresource)
    {
        var->chunk = servs.at(sock_srv).clts.at(sock_clt).rest;
        var->finish.assign("true");
    }
    var->buffer = new char[var->chunk];
    var->file.seekg (servs.at(sock_srv).clts.at(sock_clt).current_position, std::ios::beg);
    var->file.read (var->buffer, var->chunk);
    if (write(sock_clt, var->buffer, var->chunk) != var->chunk || var->finish == "true")
    {
        var->file.close();
        close(sock_clt);
        servs.at(sock_srv).clts.erase(sock_clt);
        delete[] var->buffer;
        delete var;
        return ;
    }
    servs.at(sock_srv).clts.at(sock_clt).current_position += var->chunk;
    var->file.close();
    delete[] var->buffer;
    delete var;
}

void    runCGI(int sock_clt, int sock_srv, t_getVariables *var)
{
    std::vector<std::string> out;

    std::cout << "gonna run cgi \n";
    f_cgi(sock_srv, sock_clt, servs.at(sock_srv).clts.at(sock_clt).path);
    std::cout << "|" << servs.at(sock_srv).clts.at(sock_clt).path << "|\n";
	if (servs.at(sock_srv).clts.at(sock_clt).first_time_cgi != 0)
    {
        return ;
    }
    servs.at(sock_srv).clts.at(sock_clt).path.assign(servs.at(sock_srv).clts.at(sock_clt).file_cgi);
    std::cout << "|" << servs.at(sock_srv).clts.at(sock_clt).path.c_str() << "|\n";
    if (servs.at(sock_srv).clts.at(sock_clt).type_cgi.compare("py") == 0)
        response["Content-Type: "] = "text/html";
    else if (servs.at(sock_srv).clts.at(sock_clt).type_cgi.compare("php") == 0)
    {
        std::fstream	myfile;
        std::string		str;

        var->file.open (servs.at(sock_srv).clts.at(sock_clt).path.c_str(), std::ios::binary);
        myfile.open("file", std::ios::in | std::ios::out | std::ios::app);
        while(getline(var->file, str))
        {   
            ft_split(str.c_str(), ':', out);
            if (out[0].compare("Content-type") == 0)
			{
				out[1].erase(0, 1);
				out[1].erase(24);
                response["Content-Type: "] = out[1];
			}
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
        var->file.close();
		std::cout << "|" << servs.at(sock_srv).clts.at(sock_clt).path.c_str() << "|\n";
        rename("file", servs.at(sock_srv).clts.at(sock_clt).path.c_str());
    }
    var->file.open (servs.at(sock_srv).clts.at(sock_clt).path.c_str(), std::ios::binary);
}

void    uriWithoutSlash(int sock_clt, int sock_srv)
{
	std::cout << "dir without / ... \n";
	servs.at(sock_srv).clts.at(sock_clt).path.assign(servs.at(sock_srv).clts.at(sock_clt).request_map["uri_old"]);
	servs.at(sock_srv).clts.at(sock_clt).path.append("/");
	std::cout << "path = " <<  servs.at(sock_srv).clts.at(sock_clt).path << "\n";
	servs.at(sock_srv).clts.at(sock_clt).err = "301";
	servs.at(sock_srv).clts.at(sock_clt).err_msg = "Moved Permanently";
	response["Location: "] = servs.at(sock_srv).clts.at(sock_clt).path;
	send_header(sock_clt, sock_srv, 0, NULL);
}

void    getMethod(int sock_clt, int sock_srv)
{
    t_getVariables  *var = new t_getVariables;
    struct stat     buf;
    
    if (servs.at(sock_srv).clts.at(sock_clt).new_client == 0)
    {
        initializeVariables(sock_clt, sock_srv, var);
        servs.at(sock_srv).clts.at(sock_clt).path.assign(servs.at(sock_srv).clts.at(sock_clt).request_map["uri_new"]);
        if (lstat(servs.at(sock_srv).clts.at(sock_clt).path.c_str(), &buf) == -1)
            {interruptResponse(sock_clt, sock_srv, "404", "Not Found"); delete var; return ;}
        if(S_ISREG(buf.st_mode)) // file
        {
            if (access(servs.at(sock_srv).clts.at(sock_clt).path.c_str(), R_OK) != 0)
            	{interruptResponse(sock_clt, sock_srv, "403", "Forbidden"); delete var; return ;}
        	else if (var->cgiState == "off" || (var->cgiState == "on" && var->cgiExtension == "false"))
                var->file.open (servs.at(sock_srv).clts.at(sock_clt).path.c_str(), std::ios::binary);
			else
            {
                runCGI(sock_clt, sock_srv, var);
                if (var->finish == "true")
				{
					delete var;
                    return ;
				}
            }
        }
        else // dir
        {
			if (servs.at(sock_srv).clts.at(sock_clt).path.at(servs.at(sock_srv).clts.at(sock_clt).path.size() - 1) != '/')
			{
				uriWithoutSlash(sock_clt, sock_srv);
                delete var;
                return ;
			}
            else
            {   
                std::cout << "dir with / ... \n";
                int file_exist = 0;
                if (var->indexInConfigFile && (var->cgiState == "off" || (var->cgiState == "on" &&  var->indexExtension == "false")))
                {
					std::cout << "|indexInConfigFile = " <<  var->indexInConfigFile << "|\n";
					std::cout << "|cgiState = " <<  var->cgiState << "|\n";
					std::cout << "indexExtension = " <<  var->indexExtension << "|\n";
					std::cout << "|" << servs.at(sock_srv).clts.at(sock_clt).path << "|\n";

                    if (lstat((servs.at(sock_srv).clts.at(sock_clt).path + var->indexName).c_str(), &buf) == -1)
                    {
                        file_exist = 1;
                    }
                    else
                    {
                        servs.at(sock_srv).clts.at(sock_clt).path.append(var->indexName);
                        std::cout << "|" << servs.at(sock_srv).clts.at(sock_clt).path << "|\n";
                        var->file.open (servs.at(sock_srv).clts.at(sock_clt).path.c_str(), std::ios::binary);
                        if (!var->file)
                            { interruptResponse(sock_clt, sock_srv, "500", "Internal Server Error"); delete var; return ;}
                    }
                }
                else if (var->indexInConfigFile && var->cgiState == "on" && var->indexExtension == "true")
                {
                    std::cout << "run cgi\n";
					std::cout << "|indexInConfigFile = " <<  var->indexInConfigFile << "|\n";
					std::cout << "|cgiState = " <<  var->cgiState << "|\n";
					std::cout << "indexExtension = " <<  var->indexExtension << "|\n";
                    std::cout << "|" << servs.at(sock_srv).clts.at(sock_clt).path.append(var->indexName) << "|\n";
                    if (lstat((servs.at(sock_srv).clts.at(sock_clt).path).c_str(), &buf) == -1)
                    {
                        file_exist = 1;
                    }
                    else
                    {
						runCGI(sock_clt, sock_srv, var);
						if (var->finish == "true")
						{
							delete var;
							return ;
						}
                    }
                }
				
                if (file_exist || !var->indexInConfigFile)
                {
					servs.at(sock_srv).clts.at(sock_clt).path.append("index.html");
					std::cout << "|" << servs.at(sock_srv).clts.at(sock_clt).path << "|\n";
					if (lstat(servs.at(sock_srv).clts.at(sock_clt).path.c_str(), &buf) == -1 || access(servs.at(sock_srv).clts.at(sock_clt).path.c_str(), R_OK) != 0)
					{
                        std::cout << "index not found \n";
						if (var->autoindex == "off")
							{ interruptResponse(sock_clt, sock_srv, "403", "Forbidden"); delete var; return ; }
						else
                    	{
                            std::cout << "gonna list \n";
                            std::string str;
							std::cout << "| = " << servs.at(sock_srv).clts.at(sock_clt).path << "|\n";
							int pos = servs.at(sock_srv).clts.at(sock_clt).path.find("index.html");
							servs.at(sock_srv).clts.at(sock_clt).path.replace(pos, 10, "");
							std::cout << "|path after remove index = " << servs.at(sock_srv).clts.at(sock_clt).path << "|\n";
							if ((var->dir = opendir (servs.at(sock_srv).clts.at(sock_clt).path.c_str())) != NULL)
							{
								str += "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><title> Index of " + servs.at(sock_srv).clts.at(sock_clt).path + \
								"</title></head><body bgcolor=\"white\"><h1>Index of " + servs.at(sock_srv).clts.at(sock_clt).path + "</h1><hr><ul><li><a href=\"../\">..</a></li>";
								while ((var->read_dir = readdir (var->dir)) != NULL)
								{
									if (var->read_dir->d_name[0] != '.')
									str += "<li><a href=\"" + std::string(var->read_dir->d_name) + "\">" + std::string(var->read_dir->d_name) + "</a></li>";
								}
								str += "</ul><hr></body></html>";
								closedir (var->dir);
								send_header(sock_clt, sock_srv, str.length(), ".html");
                                write(sock_clt, str.c_str(), str.length());
								close(sock_clt);
								servs.at(sock_srv).clts.erase(sock_clt);
                                delete var;
								return ;
							}
                    	}
					}
					else
                    {
						std::cout << "dir has index file \n";
                        std::cout <<"name = |" <<  servs.at(sock_srv).clts.at(sock_clt).path << "|\n";
                        var->file.open (servs.at(sock_srv).clts.at(sock_clt).path.c_str(), std::ios::binary);
                    }
                }
            }
        }
     
        if (servs.at(sock_srv).clts.at(sock_clt).first_time_cgi == 0)
        {
            std::cout << "path in headre = |" <<  servs.at(sock_srv).clts.at(sock_clt).path.c_str() << "|\n";
            var->file.seekg (0, std::ios::end);
            servs.at(sock_srv).clts.at(sock_clt).sizeOfReresource = var->file.tellg(); // TODO if size == -1
            servs.at(sock_srv).clts.at(sock_clt).rest = servs.at(sock_srv).clts.at(sock_clt).sizeOfReresource % var->chunk;
            send_header(sock_clt, sock_srv, servs.at(sock_srv).clts.at(sock_clt).sizeOfReresource, servs.at(sock_srv).clts.at(sock_clt).path.c_str());
            servs.at(sock_srv).clts.at(sock_clt).new_client++;
            var->file.close();
            delete var;
            return ;
        }
		delete var;
    }
    else if (servs.at(sock_srv).clts.at(sock_clt).first_time_cgi == 0)
        sendResponse(sock_clt, sock_srv, var);
}
