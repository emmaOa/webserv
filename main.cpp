# include "./includes/webserv.hpp"

std::map <std::string, std::string> response;
std::map<int, srvs_set> servs;
int exec_err;

int main(int arc, char *arg[])
{
    // std::cout << "--------------hdkfhsdg\n";
    // exit (0);
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
