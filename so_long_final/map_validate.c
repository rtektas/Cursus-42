/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validate.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 15:05:00 by rtektas           #+#    #+#             */
/*   Updated: 2025/09/17 15:05:00 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "game.h"

/* forward decl from map_validate_extra.c */
void		scan_validate_and_count(t_win *jeu);

static void	ensure_single_player(t_win *pnc)
{
	if (pnc->map->p_cnt != 1)
	{
		if (pnc->map->p_cnt == 0)
			err_print(ERR_MAP_NO_PLAYER);
		else
			err_print(ERR_MAP_MULTI_PLAYER);
		close_frame(pnc);
	}
}

static void	validate_boundaries(t_win *pnc)
{
	int	i;
	int	last;

	i = -1;
	while (pnc->map->mappin[0][++i])
	{
		if (pnc->map->mappin[0][i] != '1' || pnc->map->mappin[pnc->map->h
			- 1][i] != '1')
		{
			err_print(ERR_MAP_OPEN_BOUND);
			close_frame(pnc);
		}
	}
	i = -1;
	last = pnc->map->w - 1;
	while (pnc->map->mappin[++i])
	{
		if (pnc->map->mappin[i][0] != '1' || pnc->map->mappin[i][last] != '1')
		{
			err_print(ERR_MAP_OPEN_BOUND);
			close_frame(pnc);
		}
	}
}

static void	validate_rectangular_map(t_win *pnc)
{
	int	i;
	int	len;

	i = 0;
	len = pnc->map->w;
	while (pnc->map->mappin[i])
	{
		if ((int)ft_strlen(pnc->map->mappin[i]) != len)
		{
			err_print(ERR_MAP_NOT_RECT);
			close_frame(pnc);
		}
		i++;
	}
}

static void	ensure_single_exit(t_win *pnc)
{
	if (pnc->map->end_cnt < 1)
	{
		err_print(ERR_MAP_NO_EXIT);
		close_frame(pnc);
	}
	if (pnc->map->end_cnt > 1)
	{
		err_print(ERR_MAP_MULTI_EXIT);
		close_frame(pnc);
	}
}

void	validate_map(t_win *pnc)
{
	validate_rectangular_map(pnc);
	validate_boundaries(pnc);
	scan_validate_and_count(pnc);
	ensure_single_player(pnc);
	ensure_single_exit(pnc);
}
