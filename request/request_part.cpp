/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_part.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouazzan <iouazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 21:13:23 by iouazzan          #+#    #+#             */
/*   Updated: 2023/07/19 01:38:04 by iouazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/webserv.hpp"

bool check_allowed_chars(std::string str)
{
    const std::string allowedChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:/";

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
        if (servs.at(sock_srv).clts.at(sock_clt).request_map["Transfer-Encoding"] != "chunked") {
            servs.at(sock_srv).clts.at(sock_clt).err = 501;
            servs.at(sock_srv).clts.at(sock_clt).err_msg = "Not Implemented";
        }
    }
    if (servs.at(sock_srv).clts.at(sock_clt).request_map.find("Transfer-Encoding") == servs.at(sock_srv).clts.at(sock_clt).request_map.end() && servs.at(sock_srv).clts.at(sock_clt).request_map.find("Content-Length") == servs.at(sock_srv).clts.at(sock_clt).request_map.end()) {
        servs.at(sock_srv).clts.at(sock_clt).err = 400;
        servs.at(sock_srv).clts.at(sock_clt).err_msg = "Bad Request";
    }
    if (servs.at(sock_srv).clts.at(sock_clt).request_map["uri_old"].length() > 2048) {
        servs.at(sock_srv).clts.at(sock_clt).err = 414;
        servs.at(sock_srv).clts.at(sock_clt).err_msg = "Request-URI Too Long";
    }
    if (check_allowed_chars(servs.at(sock_srv).clts.at(sock_clt).request_map["uri_old"]) < 1) {
        servs.at(sock_srv).clts.at(sock_clt).err = 400;
        servs.at(sock_srv).clts.at(sock_clt).err_msg = "Bad Request";
    }
    match_location(sock_srv, sock_clt);
    // if (check_method(id_srv, servs.at(sock_srv).clts.at(sock_clt).request_map["uri_old"], servs.at(sock_srv).clts.at(sock_clt).request_map["method"]) < 1) {
    //     servs.at(sock_srv).clts.at(sock_clt).err = 405; 
    //     servs.at(sock_srv).clts.at(sock_clt).err_msg = " Method Not Allowed";
    // }
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

int request_part(char *buffer,int lent, int sock_clt, int sock_srv)
{
    std::cout << "request starte\n";
    if (servs.at(sock_srv).clts.at(sock_clt).is_done < 0) {
        std::fstream fd;
        std::fstream fd2;
        int i = 0;
        std::string name;
        std::string name2 = "test";
        std::string line;
        std::stringstream ss;
        int is_body = 0;
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
        // std::cout << lent << " " << strlen(buffer) << "\n";
        // char * bf = buffer.c_str();
        fd2.write(buffer , lent);
        // fd2 << std::string(buffer, lent);
        fd2.seekp(0, std::ios::beg);
        // if (is_empty(fd2)){
        //     std::cout << "empty\n";
        // }
        while (std::getline(fd2, line) && is_body == 0)
        {
            size += line.length() + 1;
            // std::cout << i << std::endl;
            if (i == 0)
                first_line(line, sock_clt, sock_srv);
            else if ((line.find(':') != std::string::npos)){
                    pars_head(line, sock_clt, sock_srv);
            }
            if (!(line.find(':') != std::string::npos) && !(i == 0)) {
                    break ;
            }
            i++;
        }
        std::string s = buffer ;
        size_t l = s.find("\r\n\r\n");
        std::string s2 = s.substr(l + 4);
        // s.substr(s.find("\r\n\r\n"));

        fd.write(s2.c_str() , lent - (l + 4));
        // std::cout << "lent : "<< s2.length() <<" : " << s2;
        // size += line.length() + 1;
        // char *bf = new char[lent - size];
       
        // std::cout << "\n lent : " << lent << "size : " << l << "re : " << (lent - (l + 4)) << std::endl;
        // // fd2.seekg(0, std::ios::beg);

        // fd2.read(bf, lent - size);
        // // std::getline(fd2, line);
        // fd.write(bf, lent - size);

        // if (std::getline(fd2, line)) {
        //     fd.write(line.c_str() , line.length());
        //     while (!line.empty())
        //     {
        //         fd.write(line.c_str() , line.length());
        //         std::getline(fd2, line);
        //     }
        // }

        
            // if (is_body == 2){
                
            //     // fd.write("\n" , 1);
            // }
            // if (is_body == 1)
            //     is_body++;
            // line.replace(line.find(line),line.length(),"");
            // i++;
        check_err_head(sock_srv, sock_clt);
        // fallocate(fd, FALLOC_FL_COLLAPSE_RANGE, 0, 900);
        if (servs.at(sock_srv).clts.at(sock_clt).request_map["method"].compare("POST") == 0) {
            servs.at(sock_srv).clts.at(sock_clt).fd_name = name;
           servs.at(sock_srv).clts.at(sock_clt).is_done = 0;
        }
        else {
            servs.at(sock_srv).clts.at(sock_clt).is_done = 1;
            // fd.close();
        }
        fd2.close();
        // std::cout << servs.at(sock_srv).clts.at(sock_clt).request_map["uri_new"] << "<<------\n";
    }
    else if (servs.at(sock_srv).clts.at(sock_clt).is_done == 0){
        std::fstream fd;
        fd.open(servs.at(sock_srv).clts.at(sock_clt).fd_name.c_str(), std::ios::in | std::ios::out | std::ios::app);
        if (!fd) {
            std::cout << "Open failed" << std::endl;
            return -2;
        }
        // file_contents.erase(found, word.length());
        if (servs.at(sock_srv).clts.at(sock_clt).request_map.find("Transfer-Encoding") != servs.at(sock_srv).clts.at(sock_clt).request_map.end())
        {
            std::cout << lent << "   hiiiiiii\n";
            fd.write(buffer , lent);
            
            // if (lent < 1024){
            //     servs.at(sock_srv).clts.at(sock_clt).is_done = 1;
            //     std::string line;
            //     std::getline(fd, line);
                
            // }
            if (lent < 1024) {
                std::fstream fd2;
                fd2.open("ttt", std::ios::in | std::ios::out | std::ios::app);
                if (!fd2) {
                    std::cout << "Open failed3" << std::endl;
                    return -2;
                }



                
                servs.at(sock_srv).clts.at(sock_clt).is_done = 1;
                int dec;
                // char *buff =  new char;
                fd.seekg(0, std::ios::beg);
                std::string f_word = word_from_file(fd, 0);
                int beg = f_word.length();
                std::cout << f_word ;
                dec = he_to_in(f_word);
                while (dec != 0)
                {
                    beg += dec + 2;
                    char buff[dec];
                    // fd.seekg(beg, std::ios::beg);
                    fd.read(buff, dec);
                    fd2.write(buff, dec);
                    // buff = NULL;
                    // char *c =  new char ;
                    // fd.read(c, 20);
                    // std::cout << "- "<< c << "\n" ;
                    f_word = word_from_file(fd, beg);
                    beg += f_word.length();
                    std::cout << f_word ;
                    dec = he_to_in(f_word);
                }
                // fd.close();
                // int result = std::rename("ttt", servs.at(sock_srv).clts.at(sock_clt).fd_name.c_str());
                // if (result != 0)
                //     std::cout << "Error renaming file." << std::endl;

                // delete(buff);
                
                // // first_word(fd);
                // // fd.erase()

                // std::fstream fd2;
                // fd2.open("ttt", std::ios::in | std::ios::out);
                // if (!fd2) {
                //     std::cout << "Open failed3" << std::endl;
                //     return -2;
                // }
                // fd.seekg(f_word.length(), std::ios::beg);
                
                // std::cout << "dec : "<< dec << " len : " << f_word.length() << "\n";
                






                // std::string word;
                // char c;
                // fd >> std::noskipws >> c;
                // word = c;
                // while (c != '\n') 
                // {
                //     fd >> std::noskipws >> c;
                //     word += c;
                //     std::cout << c << "\n";
                // }
                // std::cout << "\n first : "<< word;
                // size_t big = 0;
                // size_t len = 0;
                // len = 
                // len = fd.find("\r\n");







                
                // int len;
                // int size = 0;
                // std::string line;
                // servs.at(sock_srv).clts.at(sock_clt).is_done = 1;
                // fd.seekp(0, std::ios::beg);
                // std::fstream fd2;
                // fd2.open("ttt", std::ios::in | std::ios::out);
                // if (!fd2) {
                //     std::cout << "Open failed3" << std::endl;
                //     return -2;
                // }
                // std::getline(fd, line);
                // len = he_to_in(line);
                // std::cout << "hexa : "<< line  << "\n";
                // std::cout << "decimal : " << len  << "\n";
                // while (std::getline(fd, line))
                // { 
                //     // while (size < len)
                //     // {
                //     // }
                    
                //     size += line.length();
                //     if (size > len){
                        
                //         // size -= line.length();
                //         // std::cout << "size : "<< size << "len : " << len << "\n";
                //         // std::getline(fd, line);
                //         // std::cout << line.length() << "\n";
                //         // std::cout << line << "\n";
                        
                //         len = he_to_in(line);
                //         if (len == 0) {
                //             std::cout << "hexa : "<< line  << "\n";
                //             std::cout << "decimal : " << len  << "\n";
                //             break;
                //         }
                //         std::cout << "hexa : "<< line  << "\n";
                //         std::cout << "decimal : " << len  << "\n";
                //         size = 0;
                //         line.replace(line.begin(),line.end(), "");
                //         // break;
                //     }
                //     else
                //         fd2.write(line.c_str() , line.length());
                // }
            }
        }
        else {
            fd.write(buffer , lent);
            if (lent < 1024) 
                servs.at(sock_srv).clts.at(sock_clt).is_done = 1;
        }

    }
    return servs.at(sock_srv).clts.at(sock_clt).is_done;
}
