CC      := cc
CFLAGS  := -Wall -Wextra -Werror

# Detect platform (Darwin/macOS vs Linux/WSL)
UNAME_S := $(shell uname -s)

# MiniLibX setup per platform
ifeq ($(UNAME_S),Darwin)
	MLX_DIR   := minilibx
	MLX_LIB   := $(MLX_DIR)/libmlx.a
	MLX_INC   := -I$(MLX_DIR)
	MLX_LDFLAGS := -L$(MLX_DIR) -lmlx -framework OpenGL -framework AppKit
else
	# Linux (WSL/Ubuntu)
	MLX_DIR   := minilibx-linux
	MLX_LIB   := $(MLX_DIR)/libmlx.a
	MLX_INC   := -I$(MLX_DIR)
	# For minilibx-linux you typically need X11, zlib and math
	MLX_LDFLAGS := -L$(MLX_DIR) -lmlx -lXext -lX11 -lm -lz
endif

LIBFT_DIR   := libft
PRINTF_DIR  := printf

INCLUDES := -I. -I./gnl -I./$(LIBFT_DIR) -I./$(PRINTF_DIR) $(MLX_INC) \
			-I./minilibx-linux -I./minilibx -I./minilbx

GNL     := $(wildcard gnl/*c)
GNLOBJS := $(GNL:.c=.o)
SRCS    := main.c \
		   init_game.c \
		   movement_core.c \
		   input_hooks.c \
		   map_read.c \
		   render_map.c \
		   assets_checks.c \
		   path_validate.c \
		   read_helpers.c \
		   map_utils.c \
		   map_validate.c \
		   map_validate_extra.c \
		   assets_and_path.c \
		   playability_check.c \
		   error.c \
		   close_frame.c
OBJS    := $(SRCS:.c=.o)
NAME    := so_long

all: $(LIBFT_DIR)/libft.a $(PRINTF_DIR)/libftprintf.a $(MLX_LIB) $(NAME)

$(LIBFT_DIR)/libft.a:
	$(MAKE) -C $(LIBFT_DIR)

$(PRINTF_DIR)/libftprintf.a:
	$(MAKE) -C $(PRINTF_DIR)

$(MLX_LIB):
	@if [ -d "$(MLX_DIR)" ]; then \
		$(MAKE) -C $(MLX_DIR); \
	elif [ -d "minilibx-linux" ]; then \
		$(MAKE) -C minilibx-linux; \
	elif [ -d "minilibx" ]; then \
		$(MAKE) -C minilibx; \
	elif [ -d "minilbx" ]; then \
		$(MAKE) -C minilbx; \
	else \
		echo "[INFO] MiniLibX not found. Please clone minilibx-linux (Linux) or minilibx (macOS) inside project dir."; \
		echo "       For Linux: git clone https://github.com/42Paris/minilibx-linux minilibx-linux && make -C minilibx-linux"; \
	fi

$(NAME): $(OBJS) $(GNLOBJS)
	$(CC) $(CFLAGS) $(OBJS) $(GNLOBJS) \
		./$(LIBFT_DIR)/libft.a ./$(PRINTF_DIR)/libftprintf.a \
		$(MLX_LDFLAGS) -Lminilibx-linux -Lminilibx -Lminilbx -o $(NAME)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJS) $(GNLOBJS)
	# remove any stale objects from old sources at repo root
	rm -f *.o
	$(MAKE) -C $(LIBFT_DIR) clean
	$(MAKE) -C $(PRINTF_DIR) clean || true
	@if [ -d "$(MLX_DIR)" ]; then $(MAKE) -C $(MLX_DIR) clean; fi

fclean: clean
	rm -rf $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean
	$(MAKE) -C $(PRINTF_DIR) fclean || true

re: fclean all

norm:
	norminette ./*.c

.PHONY: all clean fclean re norm
