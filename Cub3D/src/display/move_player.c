/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 01:13:31 by asaracut          #+#    #+#             */
/*   Updated: 2025/12/07 03:53:28 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cube.h"

void	move_player_w(t_cub *cub)
{
	cub->move.nx = cub->player.x + cub->player.dir_x * SPEED;
	cub->move.ny = cub->player.y + cub->player.dir_y * SPEED;
	cub->move.mx = (int)cub->move.nx;
	cub->move.my = (int)cub->move.ny;
}

void	move_player_s(t_cub *cub)
{
	cub->move.nx = cub->player.x - cub->player.dir_x * SPEED;
	cub->move.ny = cub->player.y - cub->player.dir_y * SPEED;
	cub->move.mx = (int)cub->move.nx;
	cub->move.my = (int)cub->move.ny;
}

void	move_player_a(t_cub *cub)
{
	cub->move.nx += cub->player.dir_y * SPEED;
	cub->move.ny += -cub->player.dir_x * SPEED;
	cub->move.mx = (int)cub->move.nx;
	cub->move.my = (int)cub->move.ny;
}

void	move_player_d(t_cub *cub)
{
	cub->move.nx += -cub->player.dir_y * SPEED;
	cub->move.ny += cub->player.dir_x * SPEED;
	cub->move.mx = (int)cub->move.nx;
	cub->move.my = (int)cub->move.ny;
}

void	move_valid_or_no(t_cub *cub)
{
	cub->move.mx = (int)cub->move.nx;
	cub->move.my = (int)cub->move.ny;
	if (cub->move.my >= 0 && cub->move.my < cub->map.height_map
		&& cub->move.mx >= 0
		&& cub->move.mx < (int)ft_strlen(cub->map.map[cub->move.my])
		&& cub->map.map[cub->move.my][cub->move.mx] != '1')
	{
		cub->player.x = cub->move.nx;
		cub->player.y = cub->move.ny;
		render_frame(cub, cub->img.mlx_ptr, cub->img.win_ptr);
	}
}
