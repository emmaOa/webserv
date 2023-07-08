#ifndef REQUEST_HPP
# define REQUEST_HPP

int request_part(char *buffer,int lent, int sock_clt, int sock_srv);
int first_line(std::string line, int sock_clt, int sock_srv);
std::string random_String();
int pars_head(std::string line, int sock_clt, int sock_srv);
void split_one(std::string const &str, const char delim, std::vector<std::string> &out);
int nwe_path(std::string path, int sock_clt, int sock_srv);
void check_err_head(int sock_srv, int sock_clt);
int port_srv(int port, std::string host);
int check_method(int id, std::string location, std::string method);
bool check_allowed_chars(std::string str);
int match_location(int sock_srv, int sock_clt);
int check_return(int sock_srv, int sock_clt, std::string location);

#endif
