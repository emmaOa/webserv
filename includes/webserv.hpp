#ifndef WEBSERV_HPP
# define WEBSERV_HPP

#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
# include <algorithm>
#include <sys/time.h>
#include <sys/stat.h>
# include <iostream>
#include <unistd.h>
#include <signal.h>
#include <dirent.h>
#include <cstdlib>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <string>
#include <vector>
#include <deque>
#include <cmath>
#include <ctime>
#include <map>
# include <fcntl.h>
# include <stdio.h>
#include <cstdio>

# include "./parsing_file_cnf.hpp"
# include "./socket.hpp"
#include "./request.hpp"
#include "./response.hpp"
#include "./cgi.hpp"

#endif
