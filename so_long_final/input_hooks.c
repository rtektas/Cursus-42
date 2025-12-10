/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_hooks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 00:00:00 by rtektas           #+#    #+#             */
/*   Updated: 2025/11/14 00:00:00 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

static void	process_move(t_win *pnc)
{
	int	nx;
	int	ny;

	nx = pnc->chr->l_x;
	ny = pnc->chr->l_y;
	if (pnc->last_key == KEY_D)
		nx += TILE;
	if (pnc->last_key == KEY_A)
		nx -= TILE;
	if (pnc->last_key == KEY_W)
		ny -= TILE;
	if (pnc->last_key == KEY_S)
		ny += TILE;
	move_once(pnc, pnc->last_key, nx, ny);
	pnc->has_action = 0;
}

int	on_key_down(int keycode, void *param)
{
	t_win	*pnc;

	pnc = param;
	if (keycode == KEY_ESC)
		return (close_frame(pnc));
	if (keycode == KEY_ENTER)
		pnc->input.confirm = 1;
	if (keycode == KEY_D)
		pnc->input.right = 1;
	if (keycode == KEY_A)
		pnc->input.left = 1;
	if (keycode == KEY_W)
		pnc->input.up = 1;
	if (keycode == KEY_S)
		pnc->input.down = 1;
	if (!pnc->input.locked && (keycode == KEY_D || keycode == KEY_A
			|| keycode == KEY_W || keycode == KEY_S))
	{
		pnc->has_action = 1;
		pnc->last_key = keycode;
		pnc->input.locked = 1;
	}
	return (0);
}

int	game_tick(void *param)
{
	t_win	*pnc;

	pnc = param;
	if (pnc->has_action)
	{
		process_move(pnc);
	}
	if (pnc->door_open && pnc->pending_exit)
	{
		ft_printf("Win!\n");
		close_frame(pnc);
		return (0);
	}
	return (0);
}

void	draw_hud(t_win *pnc)
{
	(void)pnc;
}

int	on_key_up(int keycode, void *param)
{
	t_win	*pnc;

	pnc = param;
	if (keycode == KEY_ENTER)
		pnc->input.confirm = 0;
	if (keycode == KEY_D)
		pnc->input.right = 0;
	if (keycode == KEY_A)
		pnc->input.left = 0;
	if (keycode == KEY_W)
		pnc->input.up = 0;
	if (keycode == KEY_S)
		pnc->input.down = 0;
	if (keycode == KEY_D || keycode == KEY_A || keycode == KEY_W
		|| keycode == KEY_S)
		pnc->input.locked = 0;
	return (0);
}
