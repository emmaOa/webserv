DIR_OBJ = ./obj
NAME	=	webserv
CXX		=	c++
CXXFLAGS=
RM		=	rm -rf	
SRCS	=	main.cpp parsing_file_configuration/MainParsing.cpp \
			parsing_file_configuration/init.cpp \
			parsing_file_configuration/vld_key.cpp \
			parsing_file_configuration/check_key.cpp \
			parsing_file_configuration/check_values.cpp \
			parsing_file_configuration/store.cpp \
			socket/sockets.cpp \
			socket/clients.cpp \
			request/request_part.cpp \
			request/header_request.cpp \
			request/pars_location.cpp \
			response/get_method.cpp \
			response/utils.cpp \
			cgi/cgi.cpp \

OBJS = $(addprefix $(DIR_OBJ)/, $(SRCS:.cpp=.o))

all:		$(NAME) $(INCS)

$(NAME):	$(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
		
$(DIR_OBJ)/%.o:		%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJS) file*

fclean:		clean
	@$(RM) $(NAME) file* obj

re:			fclean all

.PHONY:		clean fclean re all
