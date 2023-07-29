/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouazzan <iouazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 16:31:30 by iouazzan          #+#    #+#             */
/*   Updated: 2023/07/29 22:13:25 by iouazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

void check_ex_cgi(std::string file, int sock_srv, int sock_clt)
{
    std::vector<std::string> out; 
    const char delim = '.';
    std::string ex;
    split_one(file, delim, out);
    
    if (out.size() > 2 && !(out.size() < 1 ))
    {
            ex += "." + out[out.size() - 1];
        std::cout << "extantion is : "<< ex << std::endl;
    }
    else if(out.size() == 2){
        ex = "." + out[1];
        std::cout << "extantion is : "<< ex << std::endl;
    }
    if (!ex.empty() && (ex.compare(".py") == 0 || ex.compare(".php") == 0)) {
        servs.at(sock_srv).clts.at(sock_clt).is_ex_cgi = 1;
        if (ex.compare(".py") == 0)
            servs.at(sock_srv).clts.at(sock_clt).type_cgi = "py";
        else
            servs.at(sock_srv).clts.at(sock_clt).type_cgi = "sh";

    }
}

void int_env_cgi(std::vector<std::pair<std::string, std::string> > &v, int sock_srv, int sock_clt)
{
    v.push_back(std::make_pair("CONTENT_TYPE=", servs.at(sock_srv).clts.at(sock_clt).request_map["CONTENT_TYPE"]));
    v.push_back(std::make_pair("CONTENT_LENGTH=", servs.at(sock_srv).clts.at(sock_clt).request_map["CONTENT_LENGTH"]));
    v.push_back(std::make_pair("REQUEST_METHOD=", servs.at(sock_srv).clts.at(sock_clt).request_map["method"]));
    v.push_back(std::make_pair("PATH_INFO=", servs.at(sock_srv).clts.at(sock_clt).request_map["uri_new"]));
    v.push_back(std::make_pair("QUERY_STRING=", servs.at(sock_srv).clts.at(sock_clt).request_map["query"]));
    v.push_back(std::make_pair("HTTP_COOKIE=", servs.at(sock_srv).clts.at(sock_clt).request_map["COOKIE"]));
    v.push_back(std::make_pair("SCRIPT_FILENAME=", servs.at(sock_srv).clts.at(sock_clt).fd_name));
    v.push_back(std::make_pair("GATEWAY_INTERFACE=", "CGI/1.1"));
    v.push_back(std::make_pair("REDIRECT_STATUS=", "200"));
    v.push_back(std::make_pair("REQUEST_URI=", servs.at(sock_srv).clts.at(sock_clt).request_map["uri_new"]));
    v.push_back(std::make_pair("HTTP_HOST=", servs.at(sock_srv).clts.at(sock_clt).request_map["Host"]));
}
void	ft_exit(std::string s)
{
	perror(s.c_str());
	exit (0);
}
std::string intToString(int num) {
    std::stringstream ss;
    ss << num;
    return ss.str();
}
int f_cgi(int sock_srv, int sock_clt)
{
    
    std::vector<std::pair<std::string, std::string> > v;
    int_env_cgi(v, sock_srv, sock_clt);
    std::string str;
    
    // for (int i = 0; i < v.size(); i++){
    //     std::cout << v[i].first << v[i].second << std::endl;
    // }    
    char *env[v.size() + 1];
    std::cout << "ahsdghlsdgjldfj-----------------------------\n";
    for (unsigned long i = 0; i < v.size(); i++)
    {
        std::string str = v[i].first + "=" + v[i].second;
        env[i] = new char[str.length() + 1];
        std::strcpy(env[i], str.c_str());
       std::cout << v[i].first << v[i].second << std::endl; 
    }
    std::cout << "ahsdghlsdgjldfj-----------------------------\n";

    env[v.size()] = nullptr; 
    int fr = fork();
	if (fr == -1)
		ft_exit("problem in first fork");
    servs.at(sock_srv).clts.at(sock_clt).file_cgi = random_String() + "_new.txt";
    std::string s = "./file_cgi/" + servs.at(sock_srv).clts.at(sock_clt).file_cgi;
    servs.at(sock_srv).clts.at(sock_clt).file_cgi = s;
	if (fr == 0) {
        std::string ex;
        if (servs.at(sock_srv).clts.at(sock_clt).type_cgi == "py")
            ex = "/usr/bin/python3";
        else
            ex = "./php-cgi";
        char *par[3];
        
        par[0] = (char *)ex.c_str();
        par[1] = (char *)servs.at(sock_srv).clts.at(sock_clt).request_map["uri_new"].c_str();
        par[2] = NULL;
        // /goinfre/iouazzan/back/csnxgn_new.txt
        std::cout << "---> " << par[0] << " - " << par[1] << std::endl;
        int fd_out = open(servs.at(sock_srv).clts.at(sock_clt).file_cgi.c_str(), O_CREAT | O_RDWR, 0644);
        if (!fd_out) {
            std::cout << "sfsf\n";
        }
        if (servs.at(sock_srv).clts.at(sock_clt).request_map["method"].compare("POST") == 0){
            int fd_in = open(servs.at(sock_srv).clts.at(sock_clt).fd_name.c_str(), O_CREAT | O_RDWR, 0644);
            dup2(fd_in, 0);
        }
	    dup2(fd_out, 1);
	    dup2(fd_out, 2);
	    close(fd_out);
        std::cout  << "-------------------------------------------->"<< par[0] << par[1] << servs.at(sock_srv).clts.at(sock_clt).file_cgi<< std::endl;
        // if (execve(servs.at(sock_srv).clts.at(sock_clt).request_map["uri_new"].c_str(), (char **)ex.c_str(), env) < 0){
        if (execve(par[0], par, env) < 0){
		    ft_exit("first execution not valid");  
        }
    }
	// close(pp->fd[0]);
    
    while
		(wait(NULL) != -1);
    for (unsigned long i = 0; i < v.size(); i++)
    {
        delete[] env[i];
    }
    return 0;
}