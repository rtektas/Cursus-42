/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_dda.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 00:17:02 by asaracut          #+#    #+#             */
/*   Updated: 2025/12/08 01:07:05 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cube.h"

static void	rf_init_dda(t_cub *cub, t_dda *da)
{
	ft_memset(da, 0, sizeof(t_dda));
	da->map_x = cub->player.x;
	da->map_y = cub->player.y;
	if (cub->render.ray_dir_x == 0.0)
		da->delta_dist_x = 1e30;
	else
		da->delta_dist_x = fabs(1.0 / cub->render.ray_dir_x);
	if (cub->render.ray_dir_y == 0.0)
		da->delta_dist_y = 1e30;
	else
		da->delta_dist_y = fabs(1.0 / cub->render.ray_dir_y);
}

static void	rf_init_steps_and_sides(t_cub *cub, t_dda *da)
{
	if (cub->render.ray_dir_x < 0)
	{
		da->step_x = -1;
		da->side_dist_x = (cub->player.x - da->map_x) * da->delta_dist_x;
	}
	else
	{
		da->step_x = 1;
		da->side_dist_x = (da->map_x + 1.0 - cub->player.x)
			* da->delta_dist_x;
	}
	if (cub->render.ray_dir_y < 0)
	{
		da->step_y = -1;
		da->side_dist_y = (cub->player.y - da->map_y) * da->delta_dist_y;
	}
	else
	{
		da->step_y = 1;
		da->side_dist_y = (da->map_y + 1.0 - cub->player.y)
			* da->delta_dist_y;
	}
}

static int	rf_dda_loop(t_cub *cub, t_dda *da)
{
	while (!da->hit)
	{
		if (da->side_dist_x < da->side_dist_y)
		{
			da->side_dist_x += da->delta_dist_x;
			da->map_x += da->step_x;
			cub->render.side = 0;
		}
		else
		{
			da->side_dist_y += da->delta_dist_y;
			da->map_y += da->step_y;
			cub->render.side = 1;
		}
		if (da->map_y < 0 || da->map_y >= cub->map.height_map
			|| da->map_x < 0
			|| da->map_x >= (int)ft_strlen(cub->map.map[da->map_y]))
			return (0);
		if (cub->map.map[da->map_y][da->map_x] == '1')
			da->hit = 1;
	}
	return (1);
}

static void	rf_set_perp_dist(t_cub *cub, t_dda *da)
{
	if (cub->render.side == 0)
		cub->render.perp_wall_dist = da->side_dist_x - da->delta_dist_x;
	else
		cub->render.perp_wall_dist = da->side_dist_y - da->delta_dist_y;
}

int	rf_wall_hit(t_cub *cub)
{
	t_dda	da;

	rf_init_dda(cub, &da);
	rf_init_steps_and_sides(cub, &da);
	if (!rf_dda_loop(cub, &da))
		return (0);
	rf_set_perp_dist(cub, &da);
	return (1);
}
