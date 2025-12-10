/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 14:45:00 by rtektas           #+#    #+#             */
/*   Updated: 2025/09/17 14:45:00 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

/* Standard library and project includes (ordered) */
# include "./libft/libft.h"
# include "./printf/ft_printf.h"
# include "get_next_line.h"
# include "mlx.h"
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
/* Tuiles logiques mappées depuis les caractères de la carte */
typedef enum e_tile
{
	TILE_EMPTY = 0,
	TILE_WALL,
	TILE_PLAYER,
	TILE_COIN,
	TILE_EXIT,
	TILE_UNKNOWN
}			t_tile;

typedef struct s_fd5
{
	int	a;
	int	b;
	int	c;
	int	d;
	int	e;
}			t_fd5;
/* Constante de taille de tuile (évite les 64 magiques partout) */
# ifndef TILE
#  define TILE 64
# endif

/* Keycodes cross-platform */
# ifdef __APPLE__
#  define KEY_A 0
#  define KEY_S 1
#  define KEY_D 2
#  define KEY_W 13
#  define KEY_ESC 53
#  define KEY_LEFT 123
#  define KEY_RIGHT 124
#  define KEY_DOWN 125
#  define KEY_UP 126
# else
#  define KEY_A 97
#  define KEY_S 115
#  define KEY_D 100
#  define KEY_W 119
#  define KEY_ESC 65307
#  define KEY_ENTER 65293
#  define KEY_LEFT 65361
#  define KEY_UP 65362
#  define KEY_RIGHT 65363
#  define KEY_DOWN 65364
# endif

typedef struct s_char
{
	int		l_x;
	int		l_y;
	int		point;
	void	*chr_front;
	void	*chr_back;
	void	*chr_left;
	void	*chr_right;
	void	*chr_cur;
}			t_char;

typedef struct s_point
{
	int		y;
	int		x;
}			t_point;

typedef struct s_dfsctx
{
	char	**g;
	int		h;
	int		w;
	int		collected;
	int		exits_seen;
}			t_dfsctx;

/* map utils */
char		**dup_map(char **src, int h, int w);
void		free_map(char **m, int h);
size_t		count_lines(const char *s);
char		*dup_seg(const char *s, size_t start, size_t len);
void		free_rows(char **arr, size_t n);
char		*join_and_free(char *a, char *b);
int			count_coins(char **rows);

typedef struct s_map
{
	char	**mappin;
	char	**mappin_copy;
	int		w;
	int		h;
	int		cb;
	int		gate_x;
	int		gate_y;
	int		p_x;
	int		p_y;
	int		p_cnt;
	int		end_cnt;
	int		coin_cnt;
}			t_map;

typedef struct s_input
{
	int		left;
	int		right;
	int		up;
	int		down;
	int		confirm;
	int		esc;
	int		locked;
}			t_input;

typedef struct s_win
{
	t_map	*map;
	t_char	*chr;
	void	*win;
	void	*mlx;
	void	*img_bg;
	void	*block_bg;
	void	*c_bg;
	void	*end_bg;
	int		height;
	int		width;
	t_input	input;
	int		has_action;
	int		last_key;
	int		door_open;
	int		pending_exit;
}			t_win;

/* Core game API (small public facades) */
t_map		*read_map(char *path);
void		render_map(t_win *pnc);
/* Redessine une seule case (coordonnées grille, pas pixels) */
void		render_cell(t_win *pnc, int gy, int gx);
/* HUD: affiche les mouvements */
void		draw_hud(t_win *pnc);
int			close_frame(void *param);
/* Minimal validation/path/assets facades */
void		validate_assets_presence(void);
void		validate_input_path(char *path);
void		validate_map(t_win *pnc);

/* BFS/playability check */
int			check_playability(t_win *jeu);

/* New input/update/render loop */
int			on_key_down(int keycode, void *param);
int			on_key_up(int keycode, void *param);
int			game_tick(void *param);
void		move_once(t_win *pnc, int side, int x, int y);

/* init */
void		start_full_game(t_win *pnc, char *path);

#endif