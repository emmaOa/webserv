#ifndef CGI_HPP
# define CGI_HPP

void check_ex_cgi(std::string file, int sock_srv, int sock_clt);
int f_cgi(int sock_srv, int sock_clt, std::string path);
std::string  check_ex_cgi_index(std::string file_name, int sock_srv, int sock_clt);
#endif
