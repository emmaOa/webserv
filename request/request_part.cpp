/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_part.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouazzan <iouazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 21:13:23 by iouazzan          #+#    #+#             */
/*   Updated: 2023/07/28 05:11:58 by iouazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

bool check_allowed_chars(std::string str)
{
    const std::string allowedChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:/._";

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
    // std::cout << "port : " << port << "host :"<< host << "\n";
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
    // int id_srv = port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host);
    if (servs.at(sock_srv).clts.at(sock_clt).request_map.find("Transfer-Encoding") != servs.at(sock_srv).clts.at(sock_clt).request_map.end()) {
        if (servs.at(sock_srv).clts.at(sock_clt).request_map["Transfer-Encoding"].compare("chunked\r") != 0) {
            servs.at(sock_srv).clts.at(sock_clt).err = "501";
            servs.at(sock_srv).clts.at(sock_clt).err_msg = "Not Implemented";
        }
    }
    if (servs.at(sock_srv).clts.at(sock_clt).request_map.find("Transfer-Encoding") == servs.at(sock_srv).clts.at(sock_clt).request_map.end() && servs.at(sock_srv).clts.at(sock_clt).request_map.find("Content-Length") == servs.at(sock_srv).clts.at(sock_clt).request_map.end() && servs.at(sock_srv).clts.at(sock_clt).request_map["method"].compare("POST") == 0) {
        servs.at(sock_srv).clts.at(sock_clt).err = "400";
        servs.at(sock_srv).clts.at(sock_clt).err_msg = "Bad Request1";
    }
    if (servs.at(sock_srv).clts.at(sock_clt).request_map["uri_old"].length() > 2048) {
        servs.at(sock_srv).clts.at(sock_clt).err = "414";
        servs.at(sock_srv).clts.at(sock_clt).err_msg = "Request-URI Too Long";
    }
    if (check_allowed_chars(servs.at(sock_srv).clts.at(sock_clt).request_map["uri_old"]) < 1) {
        servs.at(sock_srv).clts.at(sock_clt).err = "400";
        servs.at(sock_srv).clts.at(sock_clt).err_msg = "Bad Request2";
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
        // std::cout << c << "\n";
    }
    

    // int len = end - big;
    // char *c =  new char ;
    // fd.seekg(big, std::ios::beg);
    // fd.read(c, len);
    // // std::cout << "word : " << c ;
    // word = c;
    // delete(c);
    return word;
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
                // std::cout << servs.at(sock_srv).clts.at(sock_clt).request_map[out3[0]] << "\n";
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

int request_part(char *buffer,int lent, int sock_clt, int sock_srv)
{
    std::cout << "\n-------------------------------- REQUEST PART : --------------------------------\n";
    std::cout << "sock_clt" << sock_clt << "\n";
    std::cout << "sock_srv" << sock_srv << "\n";
    std::cout << "request starte\n";
    // std::cout << "-----------------------------------------------------------------------------------\n";
    if (servs.at(sock_srv).clts.at(sock_clt).is_done < 0) {
        std::fstream fd;
        std::fstream fd2;
        std::string name;
        std::string name2 = "test";
        std::string line;
        std::stringstream ss;
        int id_srv = port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host);
        int i = 0;
        int size = 0;
    
        ss << sock_clt;
        name = "file_" + ss.str() + random_String();
        fd.open(name.c_str(), std::ios::in | std::ios::out | std::ios::app);
        fd2.open(name2.c_str(),  std::ios::in | std::ios::out);
        if (!fd ) {
            std::cout << "Open failed" << std::endl;
            return -2;
        }
        if (!fd2) {
            std::cout << "Open failed2" << std::endl;
            return -2;
        }
        fd2.write(buffer , lent);
        fd2.seekp(0, std::ios::beg);
        int j = 0;
        std::getline(fd2, line);
        first_line(line, sock_clt, sock_srv);
        if (match_location(sock_srv, sock_clt) > 0) {
            servs.at(sock_srv).clts.at(sock_clt).is_done = 1;
            return 1;
        }
        while (std::getline(fd2, line))
        {
            size += line.length() + 1;
            if (j == 2)
            {
                if (line.find(':') != std::string::npos &&\
                    data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at(servs.at(sock_srv).clts.at(sock_clt).location).at("cgi_is").at(0).compare("off") == 0) {
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
                    // std::cout << line.length() << std::endl;
                    servs.at(sock_srv).clts.at(sock_clt).len_bound = line.length() + 5;
                }
                if (j > 2)
                    break ;

            }
            i++;
        }
        std::string s = buffer ;
        size_t l = s.find("\r\n\r\n") + 4;
        std::string s2 = s.substr(l);

        if (servs.at(sock_srv).clts.at(sock_clt).request_map.find("Content-Type") != servs.at(sock_srv).clts.at(sock_clt).request_map.end() && \
            servs.at(sock_srv).clts.at(sock_clt).request_map["Content-Type"].find("form-data") != std:: string :: npos&&\
            data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at(servs.at(sock_srv).clts.at(sock_clt).location).at("cgi_is").at(0).compare("off") == 0){
            size_t l2 = s2.find("\r\n\r\n") + 4;
            s2 = s2.substr(l2);
            fd.write(s2.c_str() , lent - (l2 + l));
            servs.at(sock_srv).clts.at(sock_clt).is_boundary = 1;
        }
        else
            fd.write(s2.c_str() , lent - l);

        check_err_head(sock_srv, sock_clt);
        // std::cout << servs.at(sock_srv).clts.at(sock_clt).err << "\n";
        // fallocate(fd, FALLOC_FL_COLLAPSE_RANGE, 0, 900);
        if (servs.at(sock_srv).clts.at(sock_clt).request_map["method"].compare("POST") == 0) {
            servs.at(sock_srv).clts.at(sock_clt).fd_name = name;
            servs.at(sock_srv).clts.at(sock_clt).is_done = 0;
        }
        else {
            // std::cout << "uri_new = |" << servs.at(sock_srv).clts.at(sock_clt).request_map["uri_new"] << "|" << "\n";
            std::cout << "request end\n";
            std::cout << "-----------------------------------------------------------------------------------\n";
            servs.at(sock_srv).clts.at(sock_clt).is_done = 1;
            // fd.close();
        }
        fd2.close();
    }
    else if (servs.at(sock_srv).clts.at(sock_clt).is_done == 0 && servs.at(sock_srv).clts.at(sock_clt).err.compare("null") == 0){
        std::fstream fd;
        fd.open(servs.at(sock_srv).clts.at(sock_clt).fd_name.c_str(), std::ios::in | std::ios::out | std::ios::app);
        if (!fd) {
            std::cout << "Open failed" << std::endl;
            return -2;
        }
        if (lent < 1024 && servs.at(sock_srv).clts.at(sock_clt).is_boundary == 1 &&\
            data_cnf->servers.at(port_srv(servs.at(sock_srv).port, servs.at(sock_srv).host)).at(servs.at(sock_srv).clts.at(sock_clt).location).at("cgi_is").at(0).compare("off") == 0) {
            std::cout << "im heaaaaaar2\n";
            std::string str;
            str = buffer;
            str.erase(lent - servs.at(sock_srv).clts.at(sock_clt).len_bound, lent);
            fd.write(str.c_str() , lent - servs.at(sock_srv).clts.at(sock_clt).len_bound);
        }
        else
            fd.write(buffer , lent);
        if (lent < 1024) {
            servs.at(sock_srv).clts.at(sock_clt).is_done = 1;
            if (servs.at(sock_srv).clts.at(sock_clt).request_map.find("Transfer-Encoding") != servs.at(sock_srv).clts.at(sock_clt).request_map.end()) {
                std::fstream fd2;
                int dec;
                fd2.open("ttt", std::ios::in | std::ios::out | std::ios::app);
                if (!fd2) {
                    std::cout << "Open failed3" << std::endl;
                    return -2;
                }

                servs.at(sock_srv).clts.at(sock_clt).is_done = 1;
                fd.seekg(0, std::ios::beg);
                std::string f_word = word_from_file(fd, 0);
                int beg = f_word.length();
                // std::cout << f_word ;
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
                if (result != 0)
                    std::cout << "Error renaming file." << std::endl;
                // std::cout << "uri_new = |" << servs.at(sock_srv).clts.at(sock_clt).request_map["uri_new"] << "|" << "\n";
                // std::cout << "request end\n";
                // std::cout << "-----------------------------------------------------------------------------------\n";
            }
            servs.at(sock_srv).clts.at(sock_clt).err = "201";
            fd.close();
        }
    }
    return servs.at(sock_srv).clts.at(sock_clt).is_done;
}
