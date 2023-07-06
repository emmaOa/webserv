# include "../includes/webserv.hpp"
# include "../includes/parsing_file_cnf.hpp"
# include "../includes/socket.hpp"
#include "../includes/request.hpp"

int nwe_path(std::string path, int sock_clt, int sock_srv)
{
    if ((path.find('?') != std::string::npos)) {
        (void) sock_clt;
        (void) sock_srv;
        std::vector<std::string> out; 
        const char delim = '?';
        split_one(path, delim, out);
        path = out[0];
        servs.at(sock_srv).clts.at(sock_clt).request_map["query"] = out[1];
    }
    return 0;
}

void split_one(std::string const &str, const char delim, std::vector<std::string> &out) 
{ 
    // construct a stream from the string 
    std::stringstream ss(str); 
 
    std::string s; 
    while (std::getline(ss, s, delim)) { 
        out.push_back(s); 
    } 
} 

int first_line(std::string line, int sock_clt, int sock_srv)
{
    std::vector<std::string> out; 
    const char delim = ' ';
    split_one(line, delim, out);
    // int i = 0;
    // while (i < out.size())
    // {
    //     std::cout << out[i];
    //     i++;
    // }
    std::pair<std::string, std::string>pr;
 
    servs.at(sock_srv).clts.at(sock_clt).request_map["method"] = out[0];
    servs.at(sock_srv).clts.at(sock_clt).request_map["uri_old"] = out[1];
    servs.at(sock_srv).clts.at(sock_clt).request_map["http_vr"] = out[2];

    // std::vector<std::string>::iterator it;
    // for (it = out.begin(); it != out.end(); it++) { 
    //     std::cout << *it << '\n'; 
    // }
    return 0;
}

int pars_head(std::string line, int sock_clt, int sock_srv)
{
    (void) sock_clt;
    (void) sock_srv;
    std::vector<std::string> out; 
    const char delim = ':';
    split_one(line, delim, out);
    // std::cout << out[0] << out[1] << "<======\n";
    servs.at(sock_srv).clts.at(sock_clt).request_map[out[0]] = out[1];
    // std::vector<std::string>::iterator it;
    // for (it = out.begin(); it != out.end(); it++) { 
    //     std::cout << *it << '\n'; 
    // }
    return 0;
}