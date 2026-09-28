/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate_player.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 01:44:24 by asaracut          #+#    #+#             */
/*   Updated: 2025/12/07 04:02:10 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cube.h"

void	rotation(int direction, t_cub *cub)
{
	double	angle;
	double	cos_a;
	double	sin_a;
	double	old_dir_x;
	double	old_plane_x;

	if (direction == 1)
		angle = ROT_SPEED;
	else
		angle = -ROT_SPEED;
	cos_a = cos(angle);
	sin_a = sin(angle);
	old_dir_x = cub->player.dir_x;
	cub->player.dir_x = cub->player.dir_x * cos_a - cub->player.dir_y * sin_a;
	cub->player.dir_y = old_dir_x * sin_a + cub->player.dir_y * cos_a;
	old_plane_x = cub->player.plane_x;
	cub->player.plane_x = cub->player.plane_x * cos_a - cub->player.plane_y
		* sin_a;
	cub->player.plane_y = old_plane_x * sin_a + cub->player.plane_y * cos_a;
}
