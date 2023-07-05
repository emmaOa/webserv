MANDATORY=  main.cpp parsing_file_configuration/MainParsing.cpp \
			parsing_file_configuration/init.cpp \
			parsing_file_configuration/vld_key.cpp \
			parsing_file_configuration/check_key.cpp \
			parsing_file_configuration/check_values.cpp \
			parsing_file_configuration/store.cpp \
			socket/sockets.cpp \
			socket/clients.cpp \
			request/request_part.cpp \
			request/header_request.cpp \

CC=cc
FLAGS= -Wall -Werror -Wextra -std=c++98
DEBUG=-fsanitize=address -g
NAME= webserv

all: $(NAME)

$(NAME):$(MANDATORY)
	c++ $(FLAGS) $(MANDATORY) -o $(NAME)

debug:$(VRLIBFT)
	c++ $(FLAGS) $(DEBUG) $(MANDATORY) -o $(NAME) 

clean:
	rm -rf $(NAME)

fclean:
	rm -rf $(NAME)

re: fclean all