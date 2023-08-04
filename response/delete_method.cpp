/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_method.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nidor <nidor@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 16:46:46 by namine            #+#    #+#             */
/*   Updated: 2023/08/03 20:40:42 by nidor            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

int recursive_deletion(int sock_clt, int sock_srv, std::string path)
{
	std::cout << "old path << " << path << "\n";
	std::string oldPath;
	
	oldPath.assign(path);
	struct stat buf;
	DIR *dir = opendir(path.c_str());
	if (dir == NULL)
	{
		interruptResponse(sock_clt, sock_srv, "500", "Internal Server Error");
		return (1);
	}
	struct dirent *read_dir;
	
	// path.append("/");
	while ((read_dir = readdir(dir)) != NULL)
	{
		std::cout << "gonna loup \n";
		if (read_dir->d_name[0] != '.')
		{
			std::cout <<  "file name = " << read_dir->d_name<<  "\n";
			lstat(path.append(read_dir->d_name).c_str(), &buf);
			if(S_ISREG(buf.st_mode)) // file
			{
				if (!access(path.c_str(), W_OK)) // success
				{
					std::cout << "gonna delete ...\n";
					std::remove(path.c_str());
				}
				else
				{
					std::cout << "|can't delete = " << path.c_str() << "|\n";
					interruptResponse(sock_clt, sock_srv, "500", "Internal Server Error");
					return (1);
				}
			}
			else
			{
				recursive_deletion(sock_clt, sock_srv, path);
			}
		}
		std::remove(oldPath.c_str());
	}
	closedir(dir);
	return (0);
}

int deleteMethod(int sock_clt, int sock_srv)
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
			if (!recursive_deletion(sock_clt, sock_srv, servs.at(sock_srv).clts.at(sock_clt).path))
				interruptResponse(sock_clt, sock_srv, "204", "No Content");
	}
	return (1);
}
