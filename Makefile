# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/24 12:52:25 by lfarias-          #+#    #+#              #
#    Updated: 2023/11/09 13:20:52 by lfarias-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= webserv

CXX 		= c++

CXXFLAGS	= -Wall -Werror -Wextra -std=c++98 -g

TEST_BUILD 	= tests/unit_tests/build

SRC_DIR = src
OBJ_DIR = obj

CONFIG = $(addprefix config/, \
	InputHandler.cpp		\
)

IO = $(addprefix io/,    \
	TcpServerSocket.cpp 	 \
	Controller.cpp 	         \
	Client.cpp		 \
	FileSystem.cpp \
	FileReader.cpp  \
	FileWriter.cpp  \
)

UTILS = $(addprefix utils/,    \
	Logger.cpp 	 \
)

HTTP  = $(addprefix http/,  \
	HttpRequest.cpp 	    \
	HttpResponse.cpp 		\
	HttpRequestFactory.cpp  \
	HttpParser.cpp \
	HttpResponseComposer.cpp \
	HttpTime.cpp  			\
	HttpStatus.cpp			\
	MimeType.cpp			\
	Server.cpp              \
)

SRC	= $(addprefix src/, \
	$(CONFIG) 					 \
	$(HTTP)						 \
	$(UTILS)						 \
	$(IO)					 \
	main.cpp)

# OBJ			= $(SRC:.cpp=.o)
OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

all: $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

$(TEST_BUILD):
	@cd tests/unit_tests && cmake -S . -B build

test: $(TEST_BUILD)
	@cd tests/unit_tests && cmake --build build && cd build && ctest --output-on-failure

cleantest:
	rm -rf $(TEST_BUILD)

retest: cleantest test

site:
	./webserv ./config_files/default.conf

siegetest:
	./tests/siegeTest.sh

.PHONY: all fclean clean re test cleantest retest site
