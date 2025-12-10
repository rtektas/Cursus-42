/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 12:30:00 by rtektas           #+#    #+#             */
/*   Updated: 2025/09/17 12:30:00 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "game.h"

static void	init_player_sprites(t_win *pnc, int x, int y)
{
	pnc->chr->chr_front = mlx_xpm_file_to_image(pnc->mlx,
			"./sprites/player_front_64.xpm", &x, &y);
	pnc->chr->chr_back = mlx_xpm_file_to_image(pnc->mlx,
			"./sprites/player_back_64.xpm", &x, &y);
	pnc->chr->chr_right = mlx_xpm_file_to_image(pnc->mlx,
			"./sprites/player_right_64.xpm", &x, &y);
	pnc->chr->chr_left = mlx_xpm_file_to_image(pnc->mlx,
			"./sprites/player_left_64.xpm", &x, &y);
	pnc->chr->chr_cur = pnc->chr->chr_front;
}

static void	init_env_tiles(t_win *pnc, int x, int y)
{
	pnc->img_bg = mlx_xpm_file_to_image(pnc->mlx,
			"./sprites/floor_black_64.xpm", &x, &y);
	pnc->block_bg = mlx_xpm_file_to_image(pnc->mlx, "./sprites/wall_64.xpm", &x,
			&y);
	pnc->c_bg = mlx_xpm_file_to_image(pnc->mlx, "./sprites/coin_64.xpm", &x,
			&y);
	pnc->end_bg = mlx_xpm_file_to_image(pnc->mlx, "./sprites/door_64.xpm", &x,
			&y);
}

static void	validate_assets_loaded(t_win *pnc)
{
	if (!pnc->chr->chr_front || !pnc->chr->chr_back || !pnc->chr->chr_right
		|| !pnc->chr->chr_left || !pnc->img_bg || !pnc->block_bg || !pnc->c_bg
		|| !pnc->end_bg)
	{
		err_print(ERR_XPM_INVALID);
		exit(1);
	}
}

static void	init_assets(t_win *pnc, int x, int y)
{
	pnc->height = pnc->map->h * TILE;
	pnc->width = pnc->map->w * TILE;
	pnc->mlx = mlx_init();
	pnc->win = mlx_new_window(pnc->mlx, pnc->width, pnc->height, "so long");
	pnc->chr = ft_calloc(1, sizeof(t_char));
	pnc->chr->l_x = 0;
	pnc->chr->l_y = 0;
	pnc->chr->point = 0;
	init_player_sprites(pnc, x, y);
	init_env_tiles(pnc, x, y);
	validate_assets_loaded(pnc);
}

void	start_full_game(t_win *pnc, char *path)
{
	int	x;
	int	y;

	x = 0;
	y = 0;
	pnc->map = read_map(path);
	validate_map(pnc);
	if (!check_playability(pnc))
	{
		err_print(ERR_MAP_UNSOLVABLE);
		close_frame(pnc);
	}
	init_assets(pnc, x, y);
}
