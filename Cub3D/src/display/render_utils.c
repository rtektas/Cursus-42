/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 02:20:23 by asaracut          #+#    #+#             */
/*   Updated: 2025/12/07 23:39:31 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cube.h"

void	put_pixel(t_img *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->line_len + x * (img->bpp / 8));
	*(unsigned int *)dst = color;
}

void	rf_declare_vars(t_cub *cub)
{
	cub->render.img_w = 800;
	cub->render.img_h = 600;
}

void	rf_paint_bg(t_cub *cub)
{
	int	x;
	int	y;
	int	color;

	y = 0;
	while (y < cub->render.img_h)
	{
		if (y < cub->render.img_h / 2)
			color = cub->map.sky_color;
		else
			color = cub->map.floor_color;
		x = 0;
		while (x < cub->render.img_w)
		{
			put_pixel(&cub->img, x, y, color);
			x++;
		}
		y++;
	}
}

int	get_tex_color(t_tex *tex, int tex_x, int tex_y)
{
	char	*dst;

	if (tex_x < 0)
		tex_x = 0;
	if (tex_x >= tex->width)
		tex_x = tex->width - 1;
	if (tex_y < 0)
		tex_y = 0;
	if (tex_y >= tex->height)
		tex_y = tex->height - 1;
	dst = tex->addr + (tex_y * tex->line_len + tex_x * (tex->bpp / 8));
	return (*(int *)dst);
}
