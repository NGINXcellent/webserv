# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dvargas <dvargas@student.42.rio>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/24 12:52:25 by lfarias-          #+#    #+#              #
#    Updated: 2023/07/29 13:43:37 by lfarias-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= webserv

CXX 		= c++

CXXFLAGS	= -Wall -Werror -Wextra -std=c++98 -g

TEST_BUILD 	= tests/build

INPUT = $(addprefix input/, \
	InputHandler.cpp		\
)

HTTP  = $(addprefix http/,  \
	HttpRequest.cpp 	    \
	HttpResponse.cpp 		\
	HttpRequestFactory.cpp  \
	HttpResponseFactory.cpp \
	MimeType.cpp			\
	Server.cpp              \
)

SRC			= $(addprefix src/, \
	$(INPUT) 					 \
	$(HTTP)						 \
  socketClass.cpp				 \
	main.cpp)

OBJ			= $(SRC:.cpp=.o)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

all: $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

$(TEST_BUILD):
	@cd tests && cmake -S . -B build

test: $(TEST_BUILD)
	@cd tests && cmake --build build && cd build && ctest --output-on-failure

cleantest:
	rm -rf $(TEST_BUILD)

retest: cleantest test

.PHONY: all fclean clean re test cleantest retest
