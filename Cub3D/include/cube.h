/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 20:58:19 by asaracut          #+#    #+#             */
/*   Updated: 2025/12/08 01:01:59 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBE_H
# define CUBE_H

# include "../minilibx-linux/mlx.h"
# include <fcntl.h>
# include <limits.h>
# include <math.h>
# include <stdarg.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# define SPEED 0.1
# define ROT_SPEED 0.05
# define M_PI 3.14159265358979323846

/*--- Define your structs here ---*/
typedef struct s_map
{
	char	**map;

	char	*no_texture;
	char	*so_texture;
	char	*we_texture;
	char	*ea_texture;

	int		floor_color;
	int		sky_color;

	int		player_x;
	int		player_y;
	char	player_dir;

	int		width_map;
	int		height_map;

	int		parsing_step;
	int		nombre_player;
}				t_map;

typedef struct s_player
{
	double		x;
	double		y;
	double		dir_x;
	double		dir_y;
	double		plane_x;
	double		plane_y;
}				t_player;

typedef struct s_img
{
	void		*mlx_ptr;
	void		*win_ptr;
	void		*img;
	char		*addr;
	int			bpp;
	int			line_len;
	int			endian;
}				t_img;

/* explication de ces variables de mouvement dans move_player.c
 nx, ny : nouvelle position en double
 mx, my : nouvelle position en int (pour l'index de la map)*/
typedef struct s_move
{
	double		nx;
	double		ny;
	int			mx;
	int			my;
}				t_move;

typedef struct s_tex
{
	void		*img;
	char		*addr;
	int			bpp;
	int			line_len;
	int			endian;
	int			width;
	int			height;
}				t_tex;

/* structure cree que pour la norme de 42, enlever des variables
   et les mettre ici pour avoir moins de 5 functions par declaration
   elles ne veulent pas dire grand chose ici */
typedef struct s_render
{
	int		img_w;
	int		img_h;
	double	ray_dir_x;
	double	ray_dir_y;
	int		side;
	double	perp_wall_dist;
	int		loop_x;
	int		line_h;
	double	wall_x;
}				t_render;

/* structure pour dessiner une colonne de mur
   utilisee dans render_draw.c , j'ai tout mis ici pour la norme */
typedef struct s_draw
{
	int		start;
	int		end;
	int		y;
	int		tex_x;
	double	step;
	double	tex_pos;
	int		tex_y;
	int		color;
}				t_draw;

/* structure pour le DDA
   utilisee dans render_dda.c , j'ai tout mis ici pour la norme */
typedef struct s_dda
{
	int		map_x;
	int		map_y;
	double	side_dist_x;
	double	side_dist_y;
	double	delta_dist_x;
	double	delta_dist_y;
	int		step_x;
	int		step_y;
	int		hit;
}				t_dda;

typedef struct s_cub
{
	t_map		map;
	t_player	player;
	t_img		img;
	t_move		move;
	t_tex		tex_no;
	t_tex		tex_so;
	t_tex		tex_we;
	t_tex		tex_ea;
	t_render	render;
}				t_cub;

/*--- display ---*/
int				start_game(t_cub *cub);
int				handle_key(int key, t_cub *cub);
int				handle_exit(t_cub *cub);
void			render_frame(t_cub *cub, void *mlx, void *win);
void			put_pixel(t_img *img, int x, int y, int color);
int				get_tex_color(t_tex *tex, int tex_x, int tex_y);
void			load_textures(t_cub *cub);
void			rf_draw_column(t_cub *cub);

/*--- render frame ---*/
void			rf_declare_vars(t_cub *cub);
void			rf_paint_bg(t_cub *cub);
int				get_tex_color(t_tex *tex, int tex_x, int tex_y);
int				rf_wall_hit(t_cub *cub);
int				rf_wall_hit(t_cub *cub);

/*--- movement ---*/
void			rotation(int direction, t_cub *cub);
void			move_player_w(t_cub *cub);
void			move_player_s(t_cub *cub);
void			move_player_a(t_cub *cub);
void			move_player_d(t_cub *cub);
void			move_valid_or_no(t_cub *cub);

/*--- parsing.c ---*/
int				parsing(char *map, t_cub *cub);
int				empty_line(char *line);
int				skip_spaces(char **line);
size_t			line_len_no_nl(char *line);
int				stock_info(char *line, t_cub *cub);
int				stock_map(char *line, t_cub *cub);
int				stock_texture(char *line, t_cub *cub);
int				stock_color(char *line, t_cub *cub);
void			parse_map(t_cub *cub);

/*--- error_exit.c ---*/
void			exit_error(char *message, int code);
void			free_textures(t_cub *cub);
void			free_texture_exit(t_cub *cub, char *message);
void			free_map(t_cub *cub);
void			free_all(t_cub *cub);
void			free_all_exit(t_cub *cub, char *message);

/*--- utils_lib.c ---*/
size_t			ft_strlen(const char *s);
void			*ft_memset(void *b, int c, size_t len);
int				ft_strcmp(const char *s1, const char *s2);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
void			*ft_memcpy(void *dst, const void *src, size_t n);
char			*get_next_line(int fd);
int				ft_atoi(const char *str);
char			**ft_realloc_matrix(char **old, int old_size, int new_size);

/*--- init.c ---*/
void			init_cub(t_cub *cub);
void			init_player(t_cub *cub, char direction);

#endif