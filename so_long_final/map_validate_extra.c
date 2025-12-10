/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validate_extra.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 15:06:00 by rtektas           #+#    #+#             */
/*   Updated: 2025/09/17 15:06:00 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "game.h"

static t_tile	map_char_to_tile(char c)
{
	if (c == '0')
		return (TILE_EMPTY);
	if (c == '1')
		return (TILE_WALL);
	if (c == 'P')
		return (TILE_PLAYER);
	if (c == 'C')
		return (TILE_COIN);
	if (c == 'E')
		return (TILE_EXIT);
	return (TILE_UNKNOWN);
}

static void	init_counts(t_win *jeu)
{
	jeu->map->p_cnt = 0;
	jeu->map->end_cnt = 0;
	jeu->map->coin_cnt = 0;
	jeu->map->p_x = -1;
	jeu->map->p_y = -1;
}

static void	process_char(t_win *jeu, int i, int j, char c)
{
	t_tile	tile;

	tile = map_char_to_tile(c);
	if (tile == TILE_UNKNOWN)
	{
		err_print(ERR_MAP_BAD_CHARS);
		close_frame(jeu);
	}
	else if (tile == TILE_PLAYER)
	{
		jeu->map->p_cnt++;
		jeu->map->p_x = j;
		jeu->map->p_y = i;
	}
	else if (tile == TILE_EXIT)
		jeu->map->end_cnt++;
	else if (tile == TILE_COIN)
		jeu->map->coin_cnt++;
}

void	scan_validate_and_count(t_win *jeu)
{
	int	i;
	int	j;

	init_counts(jeu);
	i = 0;
	while (jeu->map->mappin[i])
	{
		j = 0;
		while (jeu->map->mappin[i][j])
		{
			process_char(jeu, i, j, jeu->map->mappin[i][j]);
			j++;
		}
		i++;
	}
	if (jeu->map->coin_cnt <= 0)
	{
		err_print(ERR_MAP_NO_COIN);
		close_frame(jeu);
	}
}
