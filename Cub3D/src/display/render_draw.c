/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_draw.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 00:18:55 by asaracut          #+#    #+#             */
/*   Updated: 2025/12/08 00:59:41 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cube.h"

static t_tex	*rf_select_tex(t_cub *cub)
{
	if (cub->render.side == 0 && cub->render.ray_dir_x > 0)
		return (&cub->tex_we);
	else if (cub->render.side == 0 && cub->render.ray_dir_x < 0)
		return (&cub->tex_ea);
	else if (cub->render.side == 1 && cub->render.ray_dir_y > 0)
		return (&cub->tex_no);
	return (&cub->tex_so);
}

static void	rf_prepare_draw(t_cub *cub, t_tex *tex, t_draw *d)
{
	d->start = -cub->render.line_h / 2 + cub->render.img_h / 2;
	if (d->start < 0)
		d->start = 0;
	d->end = cub->render.line_h / 2 + cub->render.img_h / 2;
	if (d->end >= cub->render.img_h)
		d->end = cub->render.img_h - 1;
	d->tex_x = (int)(cub->render.wall_x * (double)tex->width);
	if ((cub->render.side == 0 && cub->render.ray_dir_x > 0)
		|| (cub->render.side == 1 && cub->render.ray_dir_y < 0))
		d->tex_x = tex->width - d->tex_x - 1;
	d->step = 1.0 * tex->height / cub->render.line_h;
	d->tex_pos = (d->start - cub->render.img_h / 2
			+ cub->render.line_h / 2) * d->step;
}

/* dessiner une colonne de mur avec texture */
void	rf_draw_column(t_cub *cub)
{
	t_draw	d;
	t_tex	*tex;

	ft_memset(&d, 0, sizeof(t_draw));
	tex = rf_select_tex(cub);
	rf_prepare_draw(cub, tex, &d);
	d.y = d.start;
	while (d.y <= d.end)
	{
		d.tex_y = (int)d.tex_pos;
		if (d.tex_y < 0)
			d.tex_y = 0;
		if (d.tex_y >= tex->height)
			d.tex_y = tex->height - 1;
		d.tex_pos += d.step;
		d.color = get_tex_color(tex, d.tex_x, d.tex_y);
		put_pixel(&cub->img, cub->render.loop_x, d.y, d.color);
		d.y++;
	}
}
