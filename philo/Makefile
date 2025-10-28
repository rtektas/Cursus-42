SRCS	=	main.c args.c time.c log.c init.c forks.c \
		philosopher.c monitor.c cleanup.c
RM		=	@rm -f
GCC		=	cc
FLAGS	=	-Wall -Wextra -Werror
NAME 	=	philo
OBJS	=	$(SRCS:.c=.o)


all: $(NAME)

$(NAME): $(OBJS)
	@$(GCC) $(FLAGS) $(SRCS)  -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY:	all clean fclean re