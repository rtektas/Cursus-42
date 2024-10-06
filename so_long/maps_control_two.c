/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maps_control_two.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 17:40:24 by rtektas           #+#    #+#             */
/*   Updated: 2024/10/06 14:12:07 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minlbx.h"

void	endln_control(char line)
{
	if (line == '\n')
	{
		write(1, "Error\nEmpty Line In Map", 24);
		exit(1);
	}
}

void	coin_control(t_win *pnc)
{
	int	i;
	int	j;

	i = -1;
	j = -1;
	while (pnc->map->mappin[++i])
	{
		while (pnc->map->mappin[i][++j])
		{
			if (pnc->map->mappin[i][j] == 'C')
				pnc->map->coin_cnt = pnc->map->coin_cnt + 1;
		}
		j = 0;
	}
	if (pnc->map->coin_cnt <= 0)
	{
		write(1, "Error\nNot Found Coin in Map", 27);
		close_frame(pnc);
	}
}

void	file_control(char *path)
{
	int	fd;

	fd = open(path, O_RDWR);
	if (fd <= 0)
	{
		write(1, "Error\nWrong File Path", 21);
		close(fd);
		exit(1);
	}
	close(fd);
}

void	char_control(t_win *pnc)
{
	int	i;
	int	j;

	i = -1;
	j = -1;
	while (pnc->map->mappin[++i])
	{
		while (pnc->map->mappin[i][++j])
		{
			if (pnc->map->mappin[i][j] != '0' && pnc->map->mappin[i][j] != '1'
				&& pnc->map->mappin[i][j] != 'P'
				&& pnc->map->mappin[i][j] != 'C'
				&& pnc->map->mappin[i][j] != 'E')
			{
				write(1, "Error\nUndefined Char", 20);
				close_frame(pnc);
			}
		}
		j = 0;
	}
}
