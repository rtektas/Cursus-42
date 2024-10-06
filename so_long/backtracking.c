/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   backtracking.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 14:14:56 by rtektas           #+#    #+#             */
/*   Updated: 2024/10/06 14:17:18 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minlbx.h"

void	mpd(t_win *game)
{
	int	i;
	int	j;

	i = 0;
	game->map->mappin_copy = (char **)malloc(sizeof(char *) * game->map->h);
	while (i < game->map->h)
	{
		game->map->mappin_copy[i] = (char *)malloc(sizeof(char) * (game->map->w
					+ 1));
		j = 0;
		while (j < game->map->w)
		{
			game->map->mappin_copy[i][j] = game->map->mappin[i][j];
			if (game->map->mappin[i][j] == 'P')
			{
				game->chr->l_y = i;
				game->chr->l_x = j;
			}
			j++;
		}
		game->map->mappin_copy[i][j] = 0;
		i++;
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

int	go_nuts(t_win *game)
{
	int	i;

	i = 0;
	game->map->end_cnt = 0;
	game->map->coin_cnt = 0;
	mpd(game);
	the_great_flood(game, game->chr->l_y, game->chr->l_x);
	game->chr->l_y = 0;
	game->chr->l_x = 0;
	while (i < game->map->h)
	{
		free(game->map->mappin_copy[i]);
		i++;
	}
	free(game->map->mappin_copy);
	if (game->map->end_cnt == 1 && game->map->coin_cnt == game->map->cb)
	{
		return (1);
	}
	return (0);
}
