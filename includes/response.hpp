#ifndef RESPONSE_HPP
# define RESPONSE_HPP

void		print_request_header(int sock_clt, int sock_srv);
void		interruptResponse(int sock_clt, int sock_srv, const char *statusCode, const char *statusMessage);
void		serve_error_file(int sock_clt, int sock_srv);
int			response_part(int sock_clt, int sock_srv);
int		    deleteMethod(int sock_clt, int sock_srv);
int         postMethod(int sock_clt, int sock_srv);
std::string	getContentType(const char* path);
int			pathSecure(std::string path);
int         send_header(int sock_clt, int sock_srv, int size, const char *path);
int         proceedResponse(int sock_clt, int sock_srv);
int         getMethod(int sock_clt, int sock_srv);

#endif
