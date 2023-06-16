/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouazzan <iouazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 08:56:15 by iouazzan          #+#    #+#             */
/*   Updated: 2023/06/15 15:31:05 by iouazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "./includes/webserv.hpp"
# include "./includes/parsing_file_cnf.hpp"
# include "./includes/socket.hpp"

int main(int arc, char *arg[])
{
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
        std::cout << "open field\n";
        exit (1);
    }
    if (srvs() < 0) {
        std::cout << "open socket field\n";
        exit (1);
    }
    return 0;
}