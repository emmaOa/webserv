#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "webserv.hpp"

int request_part(char *buffer,int lent, int sock_clt, int sock_srv);

#endif