# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/24 12:52:25 by lfarias-          #+#    #+#              #
#    Updated: 2023/07/24 17:31:04 by lfarias-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= webserv 

CXX 		= c++

CXXFLAGS	= -Wall -Werror -Wextra -std=c++98 -g

TEST_BUILD 	= tests/build

APP = $(addprefix srcs/, \
	main.cpp)

SRC			= $(APP)

OBJ			= $(SRC:.cpp=.o)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

all: $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

test: $(TEST_BUILD)
	@cd tests && cmake --build build && cd build && ctest --output-on-failure

cleantest:
	rm -f $(TEST_BUILD)

retest: cleantest test

.PHONY: all fclean clean re test cleantest retest
