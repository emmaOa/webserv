#ifndef RESPONSE_HPP
# define RESPONSE_HPP

void        deleteMethod(int sock_clt, int sock_srv, std::string path, std::string resource);
int         send_header(int sock_clt, int sock_srv, int size, const char *path);
void		print_request_header(int sock_clt, int sock_srv);
void        interruptResponse(int sock_clt, int sock_srv);
void		serve_error_file(int sock_clt, int sock_srv);
int			response_part(int sock_clt, int sock_srv);
void		deleteMethod(int sock_clt, int sock_srv);
void		postMethod(int sock_clt, int sock_srv);
std::string	getContentType(const char* path);
int			pathSecure(std::string path);

#endif
