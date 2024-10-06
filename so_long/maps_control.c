/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maps_control.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 17:40:17 by rtektas           #+#    #+#             */
/*   Updated: 2024/10/06 13:44:42 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minlbx.h"

int	str_len(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	player_control(t_win *pnc)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (pnc->map->mappin[i])
	{
		while (pnc->map->mappin[i][j])
		{
			if (pnc->map->mappin[i][j] == 'P')
			{
				pnc->map->p_cnt += 1;
			}
			j++;
		}
		i++;
		j = 0;
	}
	if (pnc->map->p_cnt != 1)
	{
		write(1, "Error \nPlayer Count Error", 25);
		close_frame(pnc);
	}
}

void	wall_control(t_win *pnc)
{
	int	i;
	int	last;

	i = -1;
	while (pnc->map->mappin[0][++i])
	{
		if (pnc->map->mappin[0][i] != '1' || pnc->map->mappin[pnc->map->h
			- 1][i] != '1')
		{
			write(1, "Error \nWall Error", 17);
			close_frame(pnc);
		}
	}
	i = -1;
	last = pnc->map->w - 1;
	while (pnc->map->mappin[++i])
	{
		if (pnc->map->mappin[i][0] != '1' || pnc->map->mappin[i][last] != '1')
		{
			write(1, "Error \nWall Error", 17);
			close_frame(pnc);
		}
	}
}

void	map_size_control(t_win *pnc)
{
	int	i;
	int	len;

	i = 0;
	len = pnc->map->w;
	while (pnc->map->mappin[i])
	{
		if (str_len(pnc->map->mappin[i]) != len)
		{
			write(1, "Error \nLine Size Error", 22);
			close_frame(pnc);
		}
		i++;
	}
}

void	end_control(t_win *pnc)
{
	int	i;
	int	j;

	i = -1;
	j = -1;
	while (pnc->map->mappin[++i])
	{
		while (pnc->map->mappin[i][++j])
		{
			if (pnc->map->mappin[i][j] == 'E')
			{
				pnc->map->end_cnt = pnc->map->end_cnt + 1;
			}
		}
		j = 0;
	}
	if (pnc->map->end_cnt < 1)
	{
		write(1, "Error \nEnd Door Count Error", 27);
		close_frame(pnc);
	}
}

void	mpd(t_win *game)
{
	int i, j;
	game->map->mappin_copy = (char **)malloc(sizeof(char *) * game->map->h);
	for (i = 0; i < game->map->h; i++)
	{
		game->map->mappin_copy[i] = (char *)malloc(sizeof(char) * (game->map->w
					+ 1));
		for (j = 0; j < game->map->w; j++)
		{
			game->map->mappin_copy[i][j] = game->map->mappin[i][j];
			if (game->map->mappin[i][j] == 'P')
			{
				game->chr->l_y = i;
				game->chr->l_x = j;
			}
		}
		game->map->mappin_copy[i][j] = 0;
	}
}

int	the_great_flood(t_win *game, int y, int x)
{
	if (x < 0 || y < 0 || y >= game->map->h || x >= game->map->w)
		return (0);
	if (game->map->mappin_copy[y][x] == '0'
		|| game->map->mappin_copy[y][x] == 'C'
		|| game->map->mappin_copy[y][x] == 'E'
		|| game->map->mappin_copy[y][x] == 'P')
	{
		if (game->map->mappin_copy[y][x] == 'E')
			game->map->end_cnt++;
		if (game->map->mappin_copy[y][x] == 'C')
			game->map->coin_cnt++;
		game->map->mappin_copy[y][x] = 'V';
		the_great_flood(game, y + 1, x);
		the_great_flood(game, y - 1, x);
		the_great_flood(game, y, x + 1);
		the_great_flood(game, y, x - 1);
	}
	return (1);
}

#include <stdio.h>

int	go_nuts(t_win *game)
{
	int	i;

	game->map->end_cnt = 0;
	game->map->coin_cnt = 0;
	mpd(game);
	the_great_flood(game, game->chr->l_y, game->chr->l_x);
	game->chr->l_y = 0;
	game->chr->l_x = 0;
	for (i = 0; i < game->map->h; i++)
	{
		free(game->map->mappin_copy[i]);
	}
	free(game->map->mappin_copy);
	if (game->map->end_cnt == 1 && game->map->coin_cnt == game->map->cb)
	{
		return (1);
	}
	return (0);
}
