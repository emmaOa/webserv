#ifndef RESPONSE_HPP
# define RESPONSE_HPP

void		interruptResponse(int sock_clt, int sock_srv, const char *statusCode, const char *statusMessage);
void        ft_split(std::string const &str, const char delim, std::vector<std::string> &out) ;
void        send_header(int sock_clt, int sock_srv, long long int size, const char *path);
void		print_request_header(int sock_clt, int sock_srv);
void		serve_error_file(int sock_clt, int sock_srv);
int			proceedResponse(int sock_clt, int sock_srv);
void		response_part(int sock_clt, int sock_srv);
std::string	getContentType(const char* path);
int			pathSecure(std::string path);
void		deleteMethod(int sock_clt, int sock_srv);
void		postMethod(int sock_clt, int sock_srv);
void		getMethod(int sock_clt, int sock_srv);
extern std::map <std::string, std::string> response;

#endif
