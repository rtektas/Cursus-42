/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_frame.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 16:37:53 by marvin            #+#    #+#             */
/*   Updated: 2025/11/14 16:37:53 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "game.h"

static void	destroy_char_images(t_win *pnc)
{
	if (!pnc->chr)
		return ;
	if (pnc->chr->chr_front)
		mlx_destroy_image(pnc->mlx, pnc->chr->chr_front);
	if (pnc->chr->chr_back)
		mlx_destroy_image(pnc->mlx, pnc->chr->chr_back);
	if (pnc->chr->chr_right)
		mlx_destroy_image(pnc->mlx, pnc->chr->chr_right);
	if (pnc->chr->chr_left)
		mlx_destroy_image(pnc->mlx, pnc->chr->chr_left);
}

static void	destroy_bg_images_and_window(t_win *pnc)
{
	if (pnc->c_bg)
		mlx_destroy_image(pnc->mlx, pnc->c_bg);
	if (pnc->end_bg)
		mlx_destroy_image(pnc->mlx, pnc->end_bg);
	if (pnc->block_bg)
		mlx_destroy_image(pnc->mlx, pnc->block_bg);
	if (pnc->img_bg)
		mlx_destroy_image(pnc->mlx, pnc->img_bg);
	if (pnc->win)
		mlx_destroy_window(pnc->mlx, pnc->win);
}

static void	free_map_struct(t_win *pnc)
{
	int	i;

	if (!pnc->map)
		return ;
	if (pnc->map->mappin)
	{
		i = 0;
		while (pnc->map->mappin[i])
			free(pnc->map->mappin[i++]);
		free(pnc->map->mappin);
	}
	free(pnc->map);
}

int	close_frame(void *param)
{
	t_win	*pnc;

	pnc = (t_win *)param;
	if (!pnc)
	{
		exit(1);
	}
	if (pnc->mlx)
	{
		destroy_char_images(pnc);
		destroy_bg_images_and_window(pnc);
	}
	if (pnc->chr)
		free(pnc->chr);
	free_map_struct(pnc);
	exit(1);
	return (1);
}
