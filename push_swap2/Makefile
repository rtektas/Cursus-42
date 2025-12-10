NAME                = push_swap

LIBFT               = ./lib/libft/libft.a
INC                 = include/
SRC_DIR             = src/
OBJ_DIR             = obj/

CC                  = gcc
CFLAGS              = -Wall -Werror -Wextra -I
RM                  = rm -f

SRCS = \
	$(SRC_DIR)entry.c \
	$(SRC_DIR)args_parse.c \
	$(SRC_DIR)deque_stack.c \
	$(SRC_DIR)deque_utils.c \
	$(SRC_DIR)swap_ops.c \
	$(SRC_DIR)push_ops.c \
	$(SRC_DIR)rotate_ops.c \
	$(SRC_DIR)rrotate_ops.c \
	$(SRC_DIR)index_assign.c \
	$(SRC_DIR)tiny_sort.c \
	$(SRC_DIR)bit_radix_sort.c \
	$(SRC_DIR)error_utils.c \
	$(SRC_DIR)io_emit.c \
	$(SRC_DIR)move_log.c

OBJ                 = $(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SRCS))

start:
					@make all

$(LIBFT):
					@make -C ./lib/libft

all:                $(NAME)

$(NAME):            $(OBJ) $(LIBFT)
					@$(CC) $(CFLAGS) $(INC) $(OBJ) $(LIBFT) -o $(NAME)

$(OBJ_DIR)%.o:      $(SRC_DIR)%.c
					@mkdir -p $(@D)
					@$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
					@$(RM) -r $(OBJ_DIR)
					@make clean -C ./lib/libft

fclean:             clean
					@$(RM) $(NAME)
					@$(RM) $(LIBFT)

re:                 fclean all

.PHONY:             start all clean fclean re