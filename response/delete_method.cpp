# include "../includes/webserv.hpp"

void recursive_deletion(int sock_clt, int sock_srv, std::string path)
{
	struct dirent	*read_dir;
	DIR				*dir;
	struct stat		buf;
	
	std::cout << " path" << path << "\n";
	dir = opendir(path.c_str());
	if (dir == NULL)
	{
		std::cout << "open failed\n";
		interruptResponse(sock_clt, sock_srv, "500", "Internal Server Error");
		return ;
	}
	if (path.at(path.size() - 1) != '/')
		path.append("/");
	while ((read_dir = readdir(dir)) != NULL)
	{
		if (read_dir->d_name[0] != '.')
		{
			std::cout <<  "readir-name = " << read_dir->d_name <<  "\n";
			if (lstat(path.append(read_dir->d_name).c_str(), &buf) == -1) {
				interruptResponse(sock_clt, sock_srv, "500", "Internal Server Error");
				return ; }
			if(S_ISREG(buf.st_mode)) // file
			{
				if (!access(path.c_str(), W_OK)) // success
				{
					std::cout << "gonna delete ...\n";
					std::remove(path.c_str());
					std::cout << "path befor" << path << "\n";
					int pos = path.find(read_dir->d_name);
					std::string str(read_dir->d_name);
					path.erase(pos, str.length());
					std::cout << "path after" << path << "\n";
				}
				else
				{
					std::cout << "can't delete = " << path.c_str() << "\n";
					{ interruptResponse(sock_clt, sock_srv, "500", "Internal Server Error"); return ; }
				}
			}
			else // dir
			{
				std::vector<std::string> out;
				std::string RootFolderName = read_dir->d_name;
				std::cout << "RootFolderName " << RootFolderName << "\n";
				std::cout << "path " << path << "\n";
				recursive_deletion(sock_clt, sock_srv, path);
				std::cout << "path = " << path << "\n";
				ft_split(path, '/', out);
				int len =  out.size();
				std::cout << "len = " << len << "\n";
				int i = 0;
				path.assign(out[i]).append("/");
				i++;
				while (i < len)
				{
					std::cout << "out = " << out[i] << "\n";
					std::cout << "read_dir->d_name = " << read_dir->d_name << "\n";
					if (out[i].compare(read_dir->d_name) != 0)
					{
						path.append(out[i]).append("/");
						std::cout << "new path = " << path << "\n";
					}
					else if (out[i].compare(read_dir->d_name) == 0 && (i != len - 1))
					{
						path.append(out[i]).append("/");
						std::cout << "new path = " << path << "\n";
					}
					else
					{
						break ;
					}
					i++;
				}
				std::cout << "path after" << path << "\n";
			}
		}
	}
	closedir(dir);
	std::remove(path.c_str());
	return ;
}

void deleteMethod(int sock_clt, int sock_srv)
{
	struct stat buf;
	
	servs.at(sock_srv).clts.at(sock_clt).path.assign(servs.at(sock_srv).clts.at(sock_clt).request_map["uri_new"]);
	if (lstat(servs.at(sock_srv).clts.at(sock_clt).path.c_str(), &buf) == -1)
		interruptResponse(sock_clt, sock_srv, "404", "Not Found");
    else if(S_ISREG(buf.st_mode)) // file
    {
		if (!access(servs.at(sock_srv).clts.at(sock_clt).path.c_str(), W_OK)) // success
		{
			if(std::remove(servs.at(sock_srv).clts.at(sock_clt).path.c_str()) == 0)
				interruptResponse(sock_clt, sock_srv, "204", "No Content");
		}
		else
			interruptResponse(sock_clt, sock_srv, "403", "Forbidden");
    }
    else
	{
		if (servs.at(sock_srv).clts.at(sock_clt).path.at(servs.at(sock_srv).clts.at(sock_clt).path.size() - 1) != '/')
			interruptResponse(sock_clt, sock_srv, "409", "Conflict");
		else
		{
			servs.at(sock_srv).clts.at(sock_clt).err.assign("204");
			servs.at(sock_srv).clts.at(sock_clt).err_msg.assign("No content");
			recursive_deletion(sock_clt, sock_srv, servs.at(sock_srv).clts.at(sock_clt).path);
		}
	}
}
