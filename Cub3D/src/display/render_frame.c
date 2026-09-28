/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_frame.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 23:43:17 by asaracut          #+#    #+#             */
/*   Updated: 2025/12/08 00:48:59 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cube.h"

// Step 3: compute ray direction for column
void	rf_compute_ray(int x, t_cub *cub)
{
	double	camera_x;

	camera_x = 2.0 * x / (double)cub->render.img_w - 1.0;
	cub->render.ray_dir_x = cub->player.dir_x
		+ cub->player.plane_x * camera_x;
	cub->render.ray_dir_y = cub->player.dir_y
		+ cub->player.plane_y * camera_x;
}

void	rf_loop_columns(t_cub *cub, void *mlx, void *win)
{
	(void)win;
	cub->render.loop_x = 0;
	while (cub->render.loop_x < cub->render.img_w)
	{
		rf_compute_ray(cub->render.loop_x, cub);
		if (!rf_wall_hit(cub))
		{
			cub->render.loop_x++;
			continue ;
		}
		if (cub->render.perp_wall_dist <= 0.0001)
			cub->render.perp_wall_dist = 0.0001;
		cub->render.line_h = (int)(cub->render.img_h
				/ cub->render.perp_wall_dist);
		if (cub->render.side == 0)
			cub->render.wall_x = cub->player.y
				+ cub->render.perp_wall_dist * cub->render.ray_dir_y;
		else
			cub->render.wall_x = cub->player.x
				+ cub->render.perp_wall_dist * cub->render.ray_dir_x;
		cub->render.wall_x -= floor(cub->render.wall_x);
		rf_draw_column(cub);
		cub->render.loop_x++;
	}
	mlx_put_image_to_window(mlx, win, cub->img.img, 0, 0);
}

void	render_frame(t_cub *cub, void *mlx, void *win)
{
	rf_declare_vars(cub);
	if (cub->img.img)
		mlx_destroy_image(mlx, cub->img.img);
	cub->img.img = mlx_new_image(mlx, cub->render.img_w, cub->render.img_h);
	cub->img.addr = mlx_get_data_addr(cub->img.img, &cub->img.bpp,
			&cub->img.line_len, &cub->img.endian);
	rf_paint_bg(cub);
	rf_loop_columns(cub, mlx, win);
}
