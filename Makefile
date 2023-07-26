NAME	= webserver

CC		= c++
CFLAGS	= -Werror -Wextra -Wall -std=c++98 -g

SRC		= src/main.cpp \
			src/socketClass.cpp
OBJ		= $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o

fclean: clean
	rm -f $(NAME)

re: fclean all

test: all
	./$(NAME)

.PHONY: all re clean fclean test