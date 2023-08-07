# include "../includes/webserv.hpp"

typedef struct s_getVariables
{
    std::string		cgiState;
    std::string		cgiExtension;
    int				indexInConfigFile;
    std::string		indexName;
    std::string		autoindex;
    std::string		indexExtension;
    struct dirent	*read_dir;
    std::ifstream	file;
    long long int	size;
    int				chunk;
    char			*buffer;
    int 			i;
    int 			rest;
    DIR 			*dir;

}t_getVariables;

void initializeVariables(int sock_clt, int sock_srv, t_getVariables *var)
{
    var->chunk = 1024;
    var->i = 0;
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
    t_getVariables *var = new t_getVariables;
    struct stat buf;
    std::vector<std::string> out;
    
    initializeVariables(sock_clt, sock_srv, var);
    if (servs.at(sock_srv).clts.at(sock_clt).new_client == 0 || servs.at(sock_srv).clts.at(sock_clt).loopDetected == "true")
    {
        servs.at(sock_srv).clts.at(sock_clt).path.assign(servs.at(sock_srv).clts.at(sock_clt).request_map["uri_new"]);
        if (lstat(servs.at(sock_srv).clts.at(sock_clt).path.c_str(), &buf) == -1)
            { interruptResponse(sock_clt, sock_srv, "404", "Not Found"); return ; }
        if(S_ISREG(buf.st_mode)) // file
        {
            if (access(servs.at(sock_srv).clts.at(sock_clt).path.c_str(), W_OK) != 0)
            {
                std::cout << "access failed \n";
                { interruptResponse(sock_clt, sock_srv, "403", "Forbidden"); return ; }
            }
        	if (var->cgiState == "off" || (var->cgiState == "on" && var->cgiExtension == "false"))
            {
                var->file.open (servs.at(sock_srv).clts.at(sock_clt).path, std::ios::in | std::ios::binary | std::ios::ate);
            }
			else // reverifier
            {
                f_cgi(sock_srv, sock_clt, servs.at(sock_srv).clts.at(sock_clt).path);
                if (servs.at(sock_srv).clts.at(sock_clt).loopDetected == "true")
                    return ;
                if (servs.at(sock_srv).clts.at(sock_clt).first_time_cgi != 0)
                {
                    servs.at(sock_srv).clts.at(sock_clt).loopDetected.assign("true");
                }
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
                std::cout << "path = " <<  servs.at(sock_srv).clts.at(sock_clt).path << "\n";
				servs.at(sock_srv).clts.at(sock_clt).err = "301";
				servs.at(sock_srv).clts.at(sock_clt).err_msg = "Moved Permanently";
                response["Location: "] = servs.at(sock_srv).clts.at(sock_clt).path;
                send_header(sock_clt, sock_srv, 0, NULL);
                close(sock_clt);
                servs.at(sock_srv).clts.erase(sock_clt);
                return ;
			}
            else
            {   
                std::cout << "dir with / ... \n";
                if (var->indexInConfigFile && (var->cgiState == "off" || (var->cgiState == "on" &&  var->indexExtension == "false")))
                {
					std::cout << "|indexInConfigFile = " <<  var->indexInConfigFile << "|\n";
					std::cout << "|cgiState = " <<  var->cgiState << "|\n";
					std::cout << "indexExtension = " <<  var->indexExtension << "|\n";
					std::cout << "|" << servs.at(sock_srv).clts.at(sock_clt).path << "|\n";
                    servs.at(sock_srv).clts.at(sock_clt).path.append(var->indexName);
                    std::cout << "|" << servs.at(sock_srv).clts.at(sock_clt).path << "|\n";
                    var->file.open (servs.at(sock_srv).clts.at(sock_clt).path, std::ios::in | std::ios::binary | std::ios::ate);
                    if (!var->file)
                        { interruptResponse(sock_clt, sock_srv, "404", "Not Found"); return ;}
                }
                else if (var->indexInConfigFile && var->cgiState == "on" && var->indexExtension == "true")
                {
					// TODO reste a traiter le cas de php file !!
                    std::cout << "run cgi\n";
					std::cout << "|indexInConfigFile = " <<  var->indexInConfigFile << "|\n";
					std::cout << "|cgiState = " <<  var->cgiState << "|\n";
					std::cout << "indexExtension = " <<  var->indexExtension << "|\n";
                    std::cout << "|" << servs.at(sock_srv).clts.at(sock_clt).path << "|\n";
                    f_cgi(sock_srv, sock_clt, servs.at(sock_srv).clts.at(sock_clt).path.append(var->indexName));
                    servs.at(sock_srv).clts.at(sock_clt).type_cgi.assign("py");
                    servs.at(sock_srv).clts.at(sock_clt).path.assign(servs.at(sock_srv).clts.at(sock_clt).file_cgi);
                    var->file.open (servs.at(sock_srv).clts.at(sock_clt).path, std::ios::in | std::ios::binary | std::ios::ate);
                    if (!var->file)
                        { interruptResponse(sock_clt, sock_srv, "404", "Not Found"); return ;}
                }
                else
                {
					std::cout << "check if i have index file in dir \n";
					servs.at(sock_srv).clts.at(sock_clt).path.append("index.html");
					std::cout << "|" << servs.at(sock_srv).clts.at(sock_clt).path << "|\n";
					if (lstat(servs.at(sock_srv).clts.at(sock_clt).path.c_str(), &buf) == -1 || access(servs.at(sock_srv).clts.at(sock_clt).path.c_str(), W_OK) != 0)
					{
						if (var->autoindex == "off")
							{ interruptResponse(sock_clt, sock_srv, "403", "Forbidden"); return ; }
						else
                    	{
							std::string str;
							std::cout << "|" << servs.at(sock_srv).clts.at(sock_clt).path << "|\n";
							int pos = servs.at(sock_srv).clts.at(sock_clt).path.find("index.html");
							servs.at(sock_srv).clts.at(sock_clt).path.replace(pos,10, "");
							std::cout << "|path after remove index = " << servs.at(sock_srv).clts.at(sock_clt).path << "|\n";
							if ((var->dir = opendir (servs.at(sock_srv).clts.at(sock_clt).path.c_str())) != NULL)
							{
								str += "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><title> Index of " + servs.at(sock_srv).clts.at(sock_clt).path + \
								"</title></head><body bgcolor=\"white\"><h1>Index of " + servs.at(sock_srv).clts.at(sock_clt).path + "</h1><hr><ul><li><a href=\"../\">..</a></li>";
								while ((var->read_dir = readdir (var->dir)) != NULL)
								{
									// should i do . too ??
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
					else
                    {
						std::cout << "dir has index file \n"; // maghadich ndir cgi wtf why mabghiiiitch !!!!
                        var->file.open (servs.at(sock_srv).clts.at(sock_clt).path, std::ios::in | std::ios::binary | std::ios::ate);
                    }
                }
            }
        }
        var->size = var->file.tellg();
        std::cout << "size = " << var->size << "\n";
        var->file.seekg (0, var->file.beg);
        var->rest = var->size % var->chunk;
        std::cout << servs.at(sock_srv).clts.at(sock_clt).type_cgi << "\n";
        if (servs.at(sock_srv).clts.at(sock_clt).type_cgi == "py")
            send_header(sock_clt, sock_srv, var->size, ".html");
        else
            send_header(sock_clt, sock_srv, var->size, servs.at(sock_srv).clts.at(sock_clt).path.c_str()); // check if send header fail !
        var->file.close();
        servs.at(sock_srv).clts.at(sock_clt).new_client++;
        delete var;
        return ;
    }
    else
    {
        initializeVariables(sock_clt, sock_srv, var);
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
            delete var;
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
            delete var;
            return ;
        }
    }
    delete[] var->buffer;
    delete var;
}
