#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "webserv.hpp"

int request_part(char *buffer,int lent, int sock_clt, int sock_srv);
int first_line(std::string line, int sock_clt, int sock_srv);
std::string random_String();
int pars_head(std::string line, int sock_clt, int sock_srv);
void split_one(std::string const &str, const char delim, std::vector<std::string> &out);
int nwe_path(std::string path, int sock_clt, int sock_srv);

#endif