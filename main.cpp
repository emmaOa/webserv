/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nidor <nidor@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 08:56:15 by iouazzan          #+#    #+#             */
/*   Updated: 2023/07/02 19:35:02 by nidor            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "./includes/webserv.hpp"

std::map<int, srvs_set> servs;

int response_part(int sock_clt, int sock_srv)
{
    (void)sock_srv;
    char hello[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    write(sock_clt , hello , sizeof(hello));
    return 1;
}

int main(int arc, char *arg[])
{
    std::ifstream CnfFile;
    char buffer[1024] = {0};
    int r = 0;
    int read_ret;
    if (arc != 2){
        std::cout << "invalid number arguments\n";
        exit (1);
    }
    CnfFile.open(arg[1]);
    if (CnfFile.is_open()){
        if (MainFileCnf(CnfFile) != 0)
            exit (1);
    }
    else{
        std::cout << "open field\n";
        exit (1);
    }
    std::deque<int> socket_srv = int_socket_srvs();
    std::map<int, srvs_set>::iterator it = servs.begin();
    while (42)
    {
        it = servs.begin();
        while (it != servs.end())
        {
            // std::cout << it->first << " <<------\n";
            r = wait_on_clients(it->first);
            if (r >= 0) {
                read_ret = read(r, buffer, 1024);
                if (read_ret < 0){
                    std::cout << "read field\n";
                    exit (1);
                }
                else {
                    if (request_part(buffer, read_ret, r, it->first) > 0) {    
                        if (check_response(r) > 0){
                            if (response_part(r, it->first) > 0) {
                                close(r);
                                it->second.clts.erase(r);
                            }
                        }
                    }
                }
            }
            ++it;
        }
    }
    
    return 0;
}