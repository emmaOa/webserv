/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_method.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namine <namine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 16:46:46 by namine            #+#    #+#             */
/*   Updated: 2023/07/30 18:34:37 by namine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

void deleteMethod(int sock_clt, int sock_srv, std::string path, std::string resource)
{
	// struct dirent	*read_dir;
	std::string		file_name;
	
    if (resource.compare("file") == 0)
    {
		if (!access(path.c_str(), W_OK)) // success
		{
			if(std::remove(path.c_str()) == 0)
				interruptResponse(sock_clt, sock_srv, "204", "No Content");
		}
		else
			interruptResponse(sock_clt, sock_srv, "403", "Forbidden");
    }
    else
    {
        std::cout << "gonna DELETE the dir ..\n";
		// if (!access(path.c_str(), W_OK))
		// {
		// 	std::cout << "can aces to the folder\n";
		// 	DIR *dir = opendir(path.c_str());
			
		// 	path.append("/");
		// 	while ((read_dir = readdir(dir)) != NULL)
		// 	{
		// 		// std::cout << read_dir->d_name << "\n";
		// 		// file_name.assign(path.append(read_dir->d_name));
		// 		// if (file_name.compare(".") == 0 || file_name.compare("..") == 0)
		// 		// 	continue ;
		// 		// std::cout << "|" << file_name << "|\n";
			
		// 		if(std::remove("./public/dir/1") == 0)
		// 		break ;
		// 		// 	std::cout << "file removed\n";
		// 		// else
		// 		// 	std::cout << "cant remove\n";
		// 	}
		// 	closedir(dir);
		// }
		// else
		// {
		// 	std::cout << "can't acces to the folder\n";
		// 	interruptResponse(sock_clt, sock_srv, "403", "Forbidden");
		// } 
    }
}
