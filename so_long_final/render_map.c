/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 00:00:00 by rtektas           #+#    #+#             */
/*   Updated: 2025/11/14 00:00:00 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

static void	update_state_for_cell(t_win *pnc, char c, int x, int y)
{
	if (c == 'P')
	{
		pnc->chr->l_x = x;
		pnc->chr->l_y = y;
	}
	else if (c == 'E')
	{
		pnc->map->gate_x = x;
		pnc->map->gate_y = y;
	}
}

static void	*overlay_image_for_char(t_win *pnc, char c)
{
	if (c == '1')
		return (pnc->block_bg);
	if (c == 'C')
		return (pnc->c_bg);
	if (c == 'E')
		return (pnc->end_bg);
	return (NULL);
}

static void	put_element(char c, t_win *pnc, int x, int y)
{
	void	*overlay;

	mlx_put_image_to_window(pnc->mlx, pnc->win, pnc->img_bg, x, y);
	update_state_for_cell(pnc, c, x, y);
	overlay = overlay_image_for_char(pnc, c);
	if (overlay)
		mlx_put_image_to_window(pnc->mlx, pnc->win, overlay, x, y);
	if (c == 'P')
		mlx_put_image_to_window(pnc->mlx, pnc->win, pnc->chr->chr_cur, x, y);
}

void	render_cell(t_win *pnc, int gy, int gx)
{
	if (!pnc || !pnc->map || !pnc->map->mappin)
		return ;
	if (gy < 0 || gx < 0 || gy >= pnc->map->h || gx >= pnc->map->w)
		return ;
	put_element(pnc->map->mappin[gy][gx], pnc, gx * TILE, gy * TILE);
}

void	render_map(t_win *pnc)
{
	int		i;
	int		j;
	int		px;
	int		py;
	char	*row;

	i = 0;
	py = 0;
	while (pnc->map->mappin[i])
	{
		j = 0;
		px = 0;
		row = pnc->map->mappin[i];
		while (row[j])
		{
			put_element(row[j], pnc, px, py);
			j++;
			px += TILE;
		}
		i++;
		py += TILE;
	}
}
