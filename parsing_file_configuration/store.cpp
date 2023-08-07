# include "../includes/webserv.hpp"

int store(std::string line)
{
    std::string word;
    std::stringstream ss(line);
    mp_dq map_tmp; 
    std::string first;
    if (data_cnf->br.second == 0){
        while (ss >> word)
        {
            if (first.empty()){
                if (vld_srv_key(word) != 0)
                    return 1;
                first = word;
            }
            else
                data_cnf->dq_2.push_back(word);
        }
        if (check_srv_value(first) != 0){
            data_cnf->dq_2.clear();
            return 1;
        }
        map_tmp.insert (std::pair<std::string, dq>("null", data_cnf->dq_2));
        data_cnf->map_1.insert (std::pair<std::string, mp_dq>(first, map_tmp));
        data_cnf->dq_2.clear();
        map_tmp.clear();
    }
    else{
        if (store_location(line) != 0)
            return 1;
    }
    return 0;
}

int is_location(std::string line)
{
    std::string word;
    int equal = 0;
    std::stringstream ss(line);
    while (ss >> word){
        if (equal == 0 && word.compare("location") == 0){
            equal++;
        }
        else if (equal == 1 && word.compare("{") == 0 && !(ss >> word)){
            equal++;
        }
        else if (equal == 1 && word.compare("}") == 0){
            std::cout << "invalid form : 04\n";
            return 1;
        }
    }
    if (equal == 2)
        return 0;
    return 1;
}

int store_location(std::string line)
{
    std::string word;
    std::stringstream ss(line);
    std::string first;

    if (data_cnf->key_map1.empty()){
        if (key_location(line) != 0){
            std::cout << "invalid location header\n";
            return 1;
        }
    }
    else {
        while (ss >> word)
        {
            if (first.empty()){
                if (vld_location_key(word) != 0)
                    return 1;
                first = word;
            }
            else
                data_cnf->dq_2.push_back(word);
        }
        if (check_lct_value(first) != 0){
            data_cnf->dq_2.clear();
            return 1;
        }
        data_cnf->map_2.insert (std::pair<std::string, dq>(first, data_cnf->dq_2));
        data_cnf->dq_2.clear();
    }
    return 0;
}
