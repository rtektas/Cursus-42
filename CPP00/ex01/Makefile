# **************************************************************************** #
#                                   Makefile                                   #
# **************************************************************************** #

NAME        = phonebook

CC         = c++
CPPFLAGS    = -Wall -Wextra -Werror -std=c++98

SRC         = main.cpp Contact.cpp PhoneBook.cpp
OBJ         = $(SRC:.cpp=.o)

# --------------------------------- Rules ------------------------------------ #

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
