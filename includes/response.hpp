#ifndef RESPONSE_HPP
# define RESPONSE_HPP

int response_part(int sock_clt, int sock_srv);
std::string	getContentType(const char* path);
void	print_request_header(int sock_clt, int sock_srv);

#endif
