/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement_core.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 00:00:00 by rtektas           #+#    #+#             */
/*   Updated: 2025/11/14 00:00:00 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

static void	update_sprite(t_win *pnc, int keycode)
{
	if (keycode == KEY_D)
		pnc->chr->chr_cur = pnc->chr->chr_right;
	if (keycode == KEY_A)
		pnc->chr->chr_cur = pnc->chr->chr_left;
	if (keycode == KEY_W)
		pnc->chr->chr_cur = pnc->chr->chr_back;
	if (keycode == KEY_S)
		pnc->chr->chr_cur = pnc->chr->chr_front;
}

static void	apply_target_effects(t_win *pnc, char target)
{
	if (target == 'C')
	{
		pnc->map->cb -= 1;
		if (pnc->map->cb <= 0)
			pnc->door_open = 1;
	}
}

static void	update_map_positions(t_win *pnc, int nx, int ny)
{
	int	ogx;
	int	ogy;

	ogx = pnc->chr->l_x / TILE;
	ogy = pnc->chr->l_y / TILE;
	pnc->map->mappin[ogy][ogx] = '0';
	pnc->map->mappin[pnc->map->gate_y / TILE][pnc->map->gate_x / TILE] = 'E';
	pnc->map->mappin[ny / TILE][nx / TILE] = 'P';
}

static void	redraw_after_move(t_win *pnc, int nx, int ny)
{
	int	pgx;
	int	pgy;

	pgx = pnc->chr->l_x / TILE;
	pgy = pnc->chr->l_y / TILE;
	render_cell(pnc, pgy, pgx);
	render_cell(pnc, ny / TILE, nx / TILE);
}

void	move_once(t_win *pnc, int side, int x, int y)
{
	char	target;

	target = pnc->map->mappin[y / TILE][x / TILE];
	if (target == '1')
		return ;
	update_sprite(pnc, side);
	pnc->chr->point += 1;
	apply_target_effects(pnc, target);
	update_map_positions(pnc, x, y);
	pnc->pending_exit = (target == 'E');
	ft_printf("%d\n", pnc->chr->point);
	redraw_after_move(pnc, x, y);
	pnc->chr->l_x = x;
	pnc->chr->l_y = y;
}
