#ifndef PARSING_FILE_CNF_HPP
# define PARSING_FILE_CNF_HPP

void initial_key_srv(void);
int MainFileCnf(std::ifstream &  CnfFile);
int vld_srv_key(std::string word);
int store_location(std::string line);
int store(std::string line);
int is_location(std::string line);
int all_key_srv(void);
void init(void);
int ParseLine(std::string line);
int all_key_location(void);
void initial_key_loc(void);
int all_key_location_cgi(void);
void initial_key_loc_cgi(void);
int vld_location_key(std::string word);
int check_srv_value(std::string key);
int key_location(std::string line);
int check_lct_value(std::string key);
int close_brackets(void);

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
    bool is_cgi;
    std::pair<int, int> location;
    std::map<std::string, int> m_s_key;
    std::map<std::string, int> m_l_key;
    std::map<std::string, int> m_l_c_key;
};
extern cnf *data_cnf;

#endif
