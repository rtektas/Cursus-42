/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maps_control.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 17:40:17 by rtektas           #+#    #+#             */
/*   Updated: 2024/10/06 14:16:11 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minlbx.h"
#include <stdio.h>

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
