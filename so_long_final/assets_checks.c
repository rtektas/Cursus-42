/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assets_checks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 00:00:00 by rtektas           #+#    #+#             */
/*   Updated: 2025/11/14 00:00:00 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "game.h"

static void	close_if_pos(int fd)
{
	if (fd > 0)
		close(fd);
}

static void	close_fd5(t_fd5 f)
{
	close_if_pos(f.a);
	close_if_pos(f.b);
	close_if_pos(f.c);
	close_if_pos(f.d);
	close_if_pos(f.e);
}

void	check_xpm_player_and_door(void)
{
	t_fd5	f;

	f.a = io_open_ro("./sprites/door_64.xpm", NULL);
	f.b = io_open_ro("./sprites/player_front_64.xpm", NULL);
	f.c = io_open_ro("./sprites/player_back_64.xpm", NULL);
	f.d = io_open_ro("./sprites/player_left_64.xpm", NULL);
	f.e = io_open_ro("./sprites/player_right_64.xpm", NULL);
	if (f.a <= 0 || f.b <= 0 || f.c <= 0 || f.d <= 0 || f.e <= 0)
	{
		err_print(ERR_XPM_MISSING);
		close_fd5(f);
		exit(1);
	}
	close_fd5(f);
}
