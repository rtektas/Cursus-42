NAME			=	minishell

# Directories
LIBFT			=	./lib/libft/libft.a
INC				=	include/
SRC_DIR			=	src/
OBJ_DIR			=	obj/

# Compiler and CFlags
CC 				=	cc
CFLAGS			=	-Wall -Werror -Wextra
LDFLAGS			=	-lreadline 

# Sources
BUILTIN_FILES	=	builtin/built-in.c builtin/cd.c builtin/echo.c builtin/env.c builtin/exit.c \
					builtin/export.c builtin/pwd.c builtin/unset.c builtin/utils-export.c

EXEC_FILES		=	exec/exec.c exec/driver.c exec/child.c exec/single.c \
			exec/single_helpers.c exec/pipe.c exec/redir.c exec/utils-exec.c \
			exec/utils-exec2.c exec/utils-exec3.c exec/herdoc.c

PARSING_FILES	=	parsing/lexer.c parsing/parser.c parsing/parser_helpers.c \
					parsing/tok-list.c parsing/tok-split.c parsing/pars-redir.c \
					parsing/expander.c parsing/expander_utils.c parsing/utils-parser.c \
					parsing/utils-parser2.c

UTILS_FILES		=	utils/utils.c utils/utils2.c utils/utils3.c utils/init.c \
				utils/signals.c utils/prompt.c utils/shlvl.c

MAIN_FILES		=	main.c

MINISHELL_FILE	=	$(MAIN_FILES) $(BUILTIN_FILES) $(EXEC_FILES) $(PARSING_FILES) $(UTILS_FILES)

SRCS			= $(addprefix $(SRC_DIR), $(MINISHELL_FILE))
OBJ				= $(SRCS:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

# Build rules 
all:			$(LIBFT) $(NAME)

$(LIBFT):
				@echo "Building libft..."
				@make -C ./lib/libft
				@echo "libft built successfully!"

$(NAME):		$(OBJ) $(LIBFT)
				@echo "Building minishell..."
				$(CC) $(CFLAGS) -I $(INC) $(OBJ) $(LIBFT) -o $(NAME) $(LDFLAGS)
				@echo "minishell built successfully!"

$(OBJ_DIR)%.o:	$(SRC_DIR)%.c
				@mkdir -p $(dir $@)
				@$(CC) $(CFLAGS) -I $(INC) -c $< -o $@
				@echo "Compiled $< to $@"

clean:
				@echo "Cleaning object files..."
				@rm -f -r $(OBJ_DIR)
				@make clean -C ./lib/libft
				@echo "Cleaned successfully!"

fclean: 		clean
				@echo "Removing executable..."
				@rm -f $(NAME)
				@rm -f $(LIBFT)
				@echo "Executable removed successfully!"


re:				fclean all

.PHONY: 		all clean fclean re