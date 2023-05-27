/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MainParsing.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iouazzan <iouazzan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 08:57:24 by iouazzan          #+#    #+#             */
/*   Updated: 2023/05/27 11:23:09 by iouazzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/parsing_file_cnf.hpp"

int ParcLine(std::string line)
{
    std::deque<std::map<std::string , std::map<std::string, std::deque<std::string> > > > servers;
    
    std::cout << line << '\n';
    return 0;
}

int MainFileCnf(void)
{
    std::ifstream CnfFile;
    std::string line;
    
    CnfFile.open("parsing_file_configuration/configuration_file.txt");
    if (CnfFile.is_open()){
        while ( getline (CnfFile,line) )
            ParcLine(line);
        CnfFile.close();
    }
    else
        std::cout << "open field";
    return 0;
}