/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouazzan <iouazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 08:56:15 by iouazzan          #+#    #+#             */
/*   Updated: 2023/07/30 18:40:46 by iouazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "./includes/webserv.hpp"
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
