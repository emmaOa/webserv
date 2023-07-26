/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namine <namine@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 08:56:15 by iouazzan          #+#    #+#             */
/*   Updated: 2023/07/26 20:21:36 by namine           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "./includes/webserv.hpp"
std::map<int, srvs_set> servs;

int main(int arc, char *arg[])
{
    signal(SIGPIPE, SIG_IGN);
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
        it = servs.begin();
        while (it != servs.end())
        {
            r = wait_on_clients(it->first);
            if (r >= 0) {
                read_ret = read(r, buffer, 1024);
                if (read_ret < 0){
                    exit (1);
                }
                else
                    request_part(buffer, read_ret, r, it->first);
            }
                w = check_response(it->first);
                if (w >= 0) {
                    if (response_part(w, it->first) > 0) {
                        // close(w);
                        it->second.clts.erase(w);
                    }
                }
            ++it;
        }
    }
    return 0;
}
