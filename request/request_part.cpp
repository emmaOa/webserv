/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_part.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emma <emma@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 21:13:23 by iouazzan          #+#    #+#             */
/*   Updated: 2023/08/07 01:09:51 by emma             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

bool check_allowed_chars(std::string str)
{
    const std::string allowedChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=";

    for (std::string::size_type i = 0; i < str.length(); ++i)
    {
        if (allowedChars.find(str[i]) == std::string::npos)
        {
            return 0;
        }
    }
    return 1;
}

int port_srv(int port, std::string host)
{
    // std::cout << "port : "<< port << "host :"<< host << "\n";
    int i = 0;
    int id = -1;
    std::deque <m_mp_dq >::iterator it;
    for (it = data_cnf->servers.begin(); it != data_cnf->servers.end(); it++){
        if (strtod(data_cnf->servers.at(i).at("port").at("null").at(0).c_str(), NULL) == port && \
                data_cnf->servers.at(i).at("host").at("null").at(0).compare(host) == 0) 
            id = i;
        i++;
    }
    return id;
}

void check_err_head(int sock_srv, int sock_clt)
{
    int id_srv = port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host);
    if (servs.at(sock_srv).clts.at(sock_clt).request_map.find("Transfer-Encoding") != servs.at(sock_srv).clts.at(sock_clt).request_map.end()) {
        if (servs.at(sock_srv).clts.at(sock_clt).request_map["Transfer-Encoding"].compare("chunked\r") != 0) {
            servs.at(sock_srv).clts.at(sock_clt).err = "501";
            servs.at(sock_srv).clts.at(sock_clt).err_msg = "Not Implemented";
        }
    } else {
        if (servs.at(sock_srv).clts.at(sock_clt).request_map.find("Content-Length") != servs.at(sock_srv).clts.at(sock_clt).request_map.end()) {
            long long len = strtod(data_cnf->servers.at(id_srv).at("client_max_body_size").at("null").at(0).c_str(), NULL);
            long long c_len = strtod(servs.at(sock_srv).clts.at(sock_clt).request_map["Content-Length"].c_str(), NULL);
            if (c_len > len){
                servs.at(sock_srv).clts.at(sock_clt).err = "413";
                servs.at(sock_srv).clts.at(sock_clt).err_msg = "content too large";
            }
        }
    }
    if (servs.at(sock_srv).clts.at(sock_clt).request_map.find("Transfer-Encoding") == servs.at(sock_srv).clts.at(sock_clt).request_map.end() && servs.at(sock_srv).clts.at(sock_clt).request_map.find("Content-Length") == servs.at(sock_srv).clts.at(sock_clt).request_map.end() && servs.at(sock_srv).clts.at(sock_clt).request_map["method"].compare("POST") == 0) {
        servs.at(sock_srv).clts.at(sock_clt).err = "400";
        servs.at(sock_srv).clts.at(sock_clt).err_msg = "Bad Request";
    }
    if (servs.at(sock_srv).clts.at(sock_clt).request_map["uri_old"].length() > 2048) {
        servs.at(sock_srv).clts.at(sock_clt).err = "414";
        servs.at(sock_srv).clts.at(sock_clt).err_msg = "Request-URI Too Long";
    }
    if (check_allowed_chars(servs.at(sock_srv).clts.at(sock_clt).request_map["uri_old"]) < 1) {
        servs.at(sock_srv).clts.at(sock_clt).err = "400";
        servs.at(sock_srv).clts.at(sock_clt).err_msg = "Bad Request";
    }
    if (servs.at(sock_srv).clts.at(sock_clt).request_map["uri_old"].length() > 2048) {
        servs.at(sock_srv).clts.at(sock_clt).err = "414";
        servs.at(sock_srv).clts.at(sock_clt).err_msg = "Request-URI Too long";
    }
}

std::string random_String()
{
    std::string randomString;
    const int length = 6;

    std::srand(std::time(0));

    for (int i = 0; i < length; ++i)
    {
        char randomChar = 'a' + (std::rand() % 26);
        randomString.push_back(randomChar);
    }

    return randomString;
}

bool is_empty(std::fstream& pFile)
{
    return pFile.peek() == std::ifstream::traits_type::eof();
}

int he_to_in(std::string hex)
{
    int decimal;
    sscanf(hex.c_str(), "%x", &decimal);
    // std::cout << decimal << "\n";
    return decimal;
}

std::string first_word(std::fstream &fd)
{
    std::string word;
    char c;
    fd >> std::noskipws >> c;
    word = c;
    while (c != '\n') 
    {
        fd >> std::noskipws >> c;
        word += c;
        // std::cout << c << "\n";
    }
    // std::cout << "first : "<< word;
    return word;
}

std::string word_from_file(std::fstream &fd, int beg)
{
    std::string word;
    fd.seekg(beg, std::ios::beg);
    char c;
    fd >> std::noskipws >> c;
    word = c;
    while (c != '\n') 
    {
        fd >> std::noskipws >> c;
        word += c;
    }
    return word;
}

std::string get_extension_type(std::string type) {
    std::cout << "type : " << type << std::endl;
    std::vector<std::pair<std::string, std::string> > v;
    v.push_back(std::make_pair("text/css\r", ".css"));
    v.push_back(std::make_pair("text/csv\r", ".csv"));
    v.push_back(std::make_pair("image/gif\r", ".gif"));
    v.push_back(std::make_pair("text/html\r", ".html"));
    v.push_back(std::make_pair("image/x-icon\r", ".ico"));
    v.push_back(std::make_pair("image/jpeg\r", ".jpeg"));
    v.push_back(std::make_pair("application/javascript\r", ".js"));
    v.push_back(std::make_pair("application/json\r", ".json"));
    v.push_back(std::make_pair("image/png\r", ".png"));
    v.push_back(std::make_pair("application/pdf\r", ".pdf"));
    v.push_back(std::make_pair("text/plain\r", ".txt"));
    v.push_back(std::make_pair("image/svg+xml\r", ".svg"));
    v.push_back(std::make_pair("application/x-httpd-php\r", ".php"));

    for (unsigned long i = 0; i < v.size(); i++)
    {
        std::cout << "v : "<< v[i].first << std::endl;
        if (v[i].first.compare(type) == 0)
            return v[i].second;
    }
    return "null";
}


int pars_bound( int sock_clt, int sock_srv, std::string line)
{
    std::vector<std::string> out;

    if (line.find(';') != std::string::npos){
        std::vector<std::string> out2;
        std::vector<std::string> out3;
        const char delim = ':';
        const char delim2 = ';';
        const char delim3 = '=';
    
        split_one(line, delim, out);
        servs.at(sock_srv).clts.at(sock_clt).request_map[out[0]] = out[1];
        split_one(out[1], delim2, out2);
        for (unsigned long i = 0; i < out2.size(); i++){
            if (out2[i].find('=') != std::string::npos){   
                split_one(out2[i], delim3, out3);
                servs.at(sock_srv).clts.at(sock_clt).request_map[out3[0]] = out3[1];
                std::cout << servs.at(sock_srv).clts.at(sock_clt).request_map[out3[0]] << "\n";
                out3.clear();
            }
        }
    }
    else if (line.find(':') != std::string::npos) {
        const char delim = ':';
        split_one(line, delim, out);
        servs.at(sock_srv).clts.at(sock_clt).request_map[out[0] + "-boundary"] = out[1];
    }
    return 0;
}


int pars_chunked_body(int sock_clt, int sock_srv, std::fstream &fd)
{
    std::cout << "pars chunked body \n";
    int id_srv = port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host); 
    std::fstream fd2;
    int dec;
    fd2.open("ttt", std::ios::in | std::ios::out | std::ios::app);
    if (!fd2) {
        std::cout << "Open failed3" << std::endl;
        return -1;
    }

    servs.at(sock_srv).clts.at(sock_clt).is_done = 1;
    fd.seekg(0, std::ios::beg);
    std::string f_word = word_from_file(fd, 0);
    int beg = f_word.length();
    dec = he_to_in(f_word);
    while (dec != 0)
    {
        beg += dec + 2;
        char buff[dec];
        fd.read(buff, dec);
        fd2.write(buff, dec);
        f_word = word_from_file(fd, beg);
        beg += f_word.length();
        // std::cout << f_word ;
        dec = he_to_in(f_word);
    }
    fd.close();
    fd2.close();
    int result = std::rename("ttt", servs.at(sock_srv).clts.at(sock_clt).fd_name.c_str());
    if (result != 0) {
        std::cout << "Error renaming file." << std::endl;
        return -1;
    }
    std::cout << "request end\n";
                
    std::cout << "-----------------------------------------------------------------------------------\n";
    long long len = strtod(data_cnf->servers.at(id_srv).at("client_max_body_size").at("null").at(0).c_str(), NULL);
    if (fd.tellg() > len) {
    // servs.at(sock_srv).clts.at(sock_clt).err = "201";
        std::cout << "===============================\n";
        fd.close();
        return 1;
    }
    // servs.at(sock_srv).clts.at(sock_clt).err = "201";
    fd.close();
    std::cout << servs.at(sock_srv).clts.at(sock_clt).is_done << "<<--------------------\n";
    return servs.at(sock_srv).clts.at(sock_clt).is_done;
}

int request_part(char *buffer,int lent, int sock_clt, int sock_srv)
{
        int id_srv = port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host); 

    std::cout << "\n-------------------------------- REQUEST PART : --------------------------------\n";
    std::cout << "sock_clt" << sock_clt << "\n";
    std::cout << "sock_srv" << sock_srv << "\n";
    std::cout << "request starte\n";
    // std::cout << "-----------------------------------------------------------------------------------\n";
    if (servs.at(sock_srv).clts.at(sock_clt).is_done < 0) {
        
        std::fstream fd2;
        std::string name;
        std::string name2 = "headr";
        std::string line;
        // int is_bound_empty = 0;
        std::stringstream ss;
        // int id_srv = port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host);
        int i = 0;
        int size = 0;
    
        ss << sock_clt;
        // servs.at(sock_srv).clts.at(sock_clt).len_bound  = 53;
        fd2.open(name2.c_str(),  std::ios::in | std::ios::out | std::ios::app);
        if (!fd2) {
            std::cout << "Open failed2" << std::endl;
            return -1;
        }
        fd2.write(buffer , lent);
        fd2.seekp(0, std::ios::beg);
        int j = 0;
        std::getline(fd2, line);
        first_line(line, sock_clt, sock_srv);
        if (match_location(sock_srv, sock_clt) > 0) {
            fd2.close();
            if (std::remove(name2.c_str()) != 0)
                std::cout << "Error deleting the file\n";
            servs.at(sock_srv).clts.at(sock_clt).is_done = 1;
            return 1;
        }
                    // exit(0);
        while (std::getline(fd2, line))
        {
            size += line.length() + 1;
            if (j == 2 )
            {
                if (line.find(':') != std::string::npos) {
                    std::cout << "im heaaaaaar1\n";
                    pars_bound(sock_clt, sock_srv, line);
                }
            }
            else {
                if ((line.find(':') != std::string::npos)){
                    pars_head(line, sock_clt, sock_srv);
                }
            }
            if (!(line.find(':') != std::string::npos) && !(i == 0)) {
                j++;
                if (j == 2){
                    // if (line.find('-') == std::string::npos)
                    //     is_bound_empty = 1;
                    // std::cout << line << "<<---- " << std::endl;
                    servs.at(sock_srv).clts.at(sock_clt).boundry = line;
                    servs.at(sock_srv).clts.at(sock_clt).len_bound = line.length() + 5;
                }
            }
            if (j > 2) {
                    std::cout << "jjj\n"  ;
                    break ;
                }
            i++;
        }
        fd2.close();
        if (std::remove(name2.c_str()) != 0)
            std::cout << "Error deleting the file\n";
        check_err_head(sock_srv, sock_clt);
        if (servs.at(sock_srv).clts.at(sock_clt).request_map["method"].compare("POST") == 0) {
            if (lent < 1024)
                servs.at(sock_srv).clts.at(sock_clt).is_done = 1;
            else
                servs.at(sock_srv).clts.at(sock_clt).is_done = 0;
            std::fstream fd;
            name = "./file_post/file_" + ss.str() + random_String();
            servs.at(sock_srv).clts.at(sock_clt).fd_name = name;
            fd.open(name.c_str(), std::ios::in | std::ios::out | std::ios::app);
            if (!fd ) {
                std::cout << "Open failed-------" << std::endl;
                return -1;
            }

            std::string s = buffer ;
            size_t l = s.find("\r\n\r\n") + 4;
            s = s.substr(l);
 
            if (servs.at(sock_srv).clts.at(sock_clt).request_map.find("Content-Type") != servs.at(sock_srv).clts.at(sock_clt).request_map.end() && \
                servs.at(sock_srv).clts.at(sock_clt).request_map["Content-Type"].find("form-data") != std:: string :: npos){
                // if (is_bound_empty == 1) {
                //     servs.at(sock_srv).clts.at(sock_clt).is_done = 1;
                //     fd.close();
                //     return 1;
                // }
                std::size_t startPos = s.find("\r\n\r\n");
                if (startPos != std::string::npos) {
                    // std::string bound_pars = s.substr(0, startPos);
                    // pars_bound(sock_clt, sock_srv, bound_pars);
                    s = s.substr(startPos + 4);
                    // std::cout << bound_pars;
                    if (!s.empty()) {
                        if (s.length() == servs.at(sock_srv).clts.at(sock_clt).len_bound) {
                            std::cout << "lennnnnnnnn\n";
                            servs.at(sock_srv).clts.at(sock_clt).is_done = 1;
                            if (data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at(servs.at(sock_srv).clts.at(sock_clt).location).at("upload_is").at(0).compare("on") == 0){
                                servs.at(sock_srv).clts.at(sock_clt).err = "400";
                                servs.at(sock_srv).clts.at(sock_clt).err_msg = "Bad Request";
                            }
                            return 1;
                        }
                        fd.write(s.c_str() , s.length() - 4);
                        if (servs.at(sock_srv).clts.at(sock_clt).is_done != 0){
                            std::cout << "im heaaaaaar2\n";
                            servs.at(sock_srv).clts.at(sock_clt).is_done = 1;
                            // fd.write(buffer, lent);
                            fd.seekg(0, std::ios::end);
                            long long file_len = fd.tellg();
                            fd.seekp(0, std::ios::beg);
                            fd.close();
                            truncate(servs.at(sock_srv).clts.at(sock_clt).fd_name.c_str(), file_len - servs.at(sock_srv).clts.at(sock_clt).len_bound);
                            long long len = strtod(data_cnf->servers.at(id_srv).at("client_max_body_size").at("null").at(0).c_str(), NULL);
                            if (file_len > len) {
                                std::cout << "===============================\n";
                                fd.close();
                                return 1;
                            }
                            fd.close();
                            return servs.at(sock_srv).clts.at(sock_clt).is_done;
                        }

                                
                    }
                        // std::size_t content = s.find("\r\n\r\n");
                        // std::cout << s;
                        // if (content != std::string::npos) {
                        //     std::string cont = s.substr(0, content);
                        //     std::string bond_2 = s.substr((content + 2), s.length());

                        // std::string boundary = input.substr(0, boundaryPos);

        // Extract the content (everything after the boundary)
                        // std::string content = input.substr(boundaryPos + delimiter.length());

                        // std::cout << "----------------------\n";
                        // std::string b = s.substr(s.length() - (servs.at(sock_srv).clts.at(sock_clt).len_bound) - 2);
                        // if (b == (servs.at(sock_srv).clts.at(sock_clt).boundry + "--"))
                        //     std::cout << b;
                        // std::string t = servs.at(sock_srv).clts.at(sock_clt).boundry + "--" ;
                        // std::cout << b.length() << std::endl <<b << t.length() << std::endl << t<< std::endl;
                            // exit(0);
                        // std::cout <<s.length() << "-" << servs.at(sock_srv).clts.at(sock_clt).len_bound << s << "\n";
                    // }
                }
   
                servs.at(sock_srv).clts.at(sock_clt).is_boundary = 1;
                servs.at(sock_srv).clts.at(sock_clt).is_done = 0;

            }
            else {
                fd.write(s.c_str() , lent - l);
                if ( servs.at(sock_srv).clts.at(sock_clt).is_done == 1) {
                    fd.seekp(0, std::ios::end);
                    if (fd.tellg() == 0){
                        servs.at(sock_srv).clts.at(sock_clt).err = "400";
                        servs.at(sock_srv).clts.at(sock_clt).err_msg = "Bad Request";
                        servs.at(sock_srv).clts.at(sock_clt).is_done = 1;
                        return servs.at(sock_srv).clts.at(sock_clt).is_done;
                    } else if((servs.at(sock_srv).clts.at(sock_clt).request_map.find("Transfer-Encoding") != servs.at(sock_srv).clts.at(sock_clt).request_map.end()) && fd.tellg() != 0)
                        return pars_chunked_body(sock_clt, sock_srv, fd);
                }
            }
            // std::cout << "/////////////////////////////////\n";
            std::cout << "request end with 0\n";
            std::cout << "-----------------------------------------------------------------------------------\n";
        }
        else {
            servs.at(sock_srv).clts.at(sock_clt).is_done = 1;
            // fd.close();
        }
    }
    else if (servs.at(sock_srv).clts.at(sock_clt).is_done == 0){
        std::cout << "first in body\n";
        std::fstream fd;
        fd.open(servs.at(sock_srv).clts.at(sock_clt).fd_name.c_str(), std::ios::in | std::ios::out | std::ios::app);
        if (!fd) {
            std::cout << "Open failed"  << servs.at(sock_srv).clts.at(sock_clt).fd_name<< std::endl;
            return -1;
        }
        if (lent < 1024 && servs.at(sock_srv).clts.at(sock_clt).is_boundary == 1) {
            std::cout << "im heaaaaaar2\n";
            servs.at(sock_srv).clts.at(sock_clt).is_done = 1;
            fd.write(buffer, lent);
            fd.seekg(0, std::ios::end);
            long long file_len = fd.tellg();
            fd.seekp(0, std::ios::beg);
            fd.close();
            truncate(servs.at(sock_srv).clts.at(sock_clt).fd_name.c_str(), file_len - servs.at(sock_srv).clts.at(sock_clt).len_bound);
            long long len = strtod(data_cnf->servers.at(id_srv).at("client_max_body_size").at("null").at(0).c_str(), NULL);
            if (file_len > len) {
                std::cout << "===============================\n";
                fd.close();
                return 1;
            }
            fd.close();
            return servs.at(sock_srv).clts.at(sock_clt).is_done;
        }
        else
            fd.write(buffer , lent);
        if (lent < 1024) {
            servs.at(sock_srv).clts.at(sock_clt).is_done = 1;
            if (servs.at(sock_srv).clts.at(sock_clt).request_map.find("Transfer-Encoding") != servs.at(sock_srv).clts.at(sock_clt).request_map.end()) {
                return (pars_chunked_body(sock_clt, sock_srv, fd));
            }
            else if (servs.at(sock_srv).clts.at(sock_clt).is_boundary != 1) {
                std::cout << "im heaaaaaar1\n";
                std::cout << "ext\n";
                std::string ex = get_extension_type(servs.at(sock_srv).clts.at(sock_clt).request_map["Content-Type"]);
                std::string new_s = servs.at(sock_srv).clts.at(sock_clt).fd_name +  ex;
                std::cout << "Content-Type : "<< servs.at(sock_srv).clts.at(sock_clt).request_map["Content-Type"] << std::endl;
                std::cout << "ex : "<< servs.at(sock_srv).clts.at(sock_clt).fd_name << std::endl;
                int result = std::rename(servs.at(sock_srv).clts.at(sock_clt).fd_name.c_str(), new_s.c_str());
                if (result != 0) {
                    std::cout << "Error renaming file." << std::endl;
                    return -1;
                }
            }
            long long len = strtod(data_cnf->servers.at(id_srv).at("client_max_body_size").at("null").at(0).c_str(), NULL);
            if (fd.tellg() > len) {
                std::cout << "===============================\n";
                fd.close();
                return 1;
            }
            std::cout << "request end\n";
            std::cout << "-----------------------------------------------------------------------------------\n";
            // servs.at(sock_srv).clts.at(sock_clt).err = "201";
            fd.close();
        }
        std::cout << servs.at(sock_srv).clts.at(sock_clt).is_done << "<<--------------------\n";
    }
    return servs.at(sock_srv).clts.at(sock_clt).is_done;
}
