/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouazzan <iouazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 08:56:15 by iouazzan          #+#    #+#             */
/*   Updated: 2023/07/19 00:04:41 by iouazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "./includes/webserv.hpp"
std::map<int, srvs_set> servs;

int main(int arc, char *arg[])
{
    std::ifstream CnfFile;
    char buffer[1024] = {0};
    int r = 0;
    int w = 0;
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
        std::cout << "open failed\n";
        exit (1);
    }
    std::deque<int> socket_srv = int_socket_srvs();
    std::map<int, srvs_set>::iterator it = servs.begin();
    while (42)
    {
        // std::cout << "b inf while\n";
        it = servs.begin();
        while (it != servs.end())
        {
            // std::cout << "b srv while\n";
            r = wait_on_clients(it->first);
            // std::cout << "wait on cliennts\n";
            if (r >= 0) {
                // std::cout << "b read request\n";
                read_ret = read(r, buffer, 1024);
                // std::cout << "read request\n";
                if (read_ret < 0){
                    // std::cout << "\nread_ret\n";
                    // std::cout << "\nread_ret\n";
                    exit (1);
                }
                else
                    request_part(buffer, read_ret, r, it->first);
            }
            // else {
                // std::cout << "b check response\n";
                w = check_response(it->first);
                // std::cout << " check response\n";
                if (w >= 0) {
                    // std::cout << "b response\n";
                    if (response_part(w, it->first) > 0) {
                        // std::cout << "response\n";
                        // std::cout << "close\n";
                        close(w);
                        it->second.clts.erase(w);
                    }
                    // std::cout << "2 response\n";
                }
            // }
            // std::cout << "srv while\n";
            ++it;
        }
        // std::cout << "inf while\n";
    }
    // std::cout << "end\n";
    return 0;
}
