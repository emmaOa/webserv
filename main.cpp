/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nidor <nidor@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 14:21:16 by nidor             #+#    #+#             */
/*   Updated: 2023/08/05 14:21:21 by nidor            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "./includes/webserv.hpp"

std::map <std::string, std::string> response;
std::map<int, srvs_set> servs;
int exec_err;

int main(int arc, char *arg[])
{
    signal(SIGPIPE, SIG_IGN);
    exec_err = 0;
    std::ifstream CnfFile;
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
    while (42)
    {
        if (wait_on_clients() < 0)
            return 1;
    }
    return 0;
}
