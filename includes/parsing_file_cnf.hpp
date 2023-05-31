#ifndef PARSING_FILE_CNF_HPP
# define PARSING_FILE_CNF_HPP

# include "webserv.hpp"

int MainFileCnf(void);

typedef std::deque<std::string> dq;
typedef std::map<std::string, dq > mp_dq;
typedef std::map<std::string, mp_dq > m_mp_dq;

struct cnf
{
    std::pair<int, int> br;
    std::deque<m_mp_dq > servers;
    m_mp_dq map_1;
    mp_dq map_2;
    dq dq_2;
    std::string key_map1;
    int if_map_2;
};


#endif