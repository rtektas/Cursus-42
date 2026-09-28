/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 03:02:59 by asaracut          #+#    #+#             */
/*   Updated: 2025/12/07 03:48:39 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cube.h"

static void	load_texture(t_cub *cub, t_tex *tex, char *path)
{
	tex->img = mlx_xpm_file_to_image(cub->img.mlx_ptr, path, &tex->width,
			&tex->height);
	if (!tex->img)
		free_all_exit(cub, "Failed to load texture");
	tex->addr = mlx_get_data_addr(tex->img, &tex->bpp, &tex->line_len,
			&tex->endian);
}

void	load_textures(t_cub *cub)
{
	load_texture(cub, &cub->tex_no, cub->map.no_texture);
	load_texture(cub, &cub->tex_so, cub->map.so_texture);
	load_texture(cub, &cub->tex_we, cub->map.we_texture);
	load_texture(cub, &cub->tex_ea, cub->map.ea_texture);
}

static void	init_player_ns(t_cub *cub, char orientation)
{
	if (orientation == 'N')
	{
		cub->player.dir_x = 0;
		cub->player.dir_y = -1;
		cub->player.plane_x = 0.66;
		cub->player.plane_y = 0;
	}
	else if (orientation == 'S')
	{
		cub->player.dir_x = 0;
		cub->player.dir_y = 1;
		cub->player.plane_x = -0.66;
		cub->player.plane_y = 0;
	}
}

static void	init_player_ew(t_cub *cub, char orientation)
{
	if (orientation == 'E')
	{
		cub->player.dir_x = 1;
		cub->player.dir_y = 0;
		cub->player.plane_x = 0;
		cub->player.plane_y = 0.66;
	}
	else if (orientation == 'W')
	{
		cub->player.dir_x = -1;
		cub->player.dir_y = 0;
		cub->player.plane_x = 0;
		cub->player.plane_y = -0.66;
	}
}

void	init_player(t_cub *cub, char orientation)
{
	if (orientation == 'N' || orientation == 'S')
		init_player_ns(cub, orientation);
	else if (orientation == 'E' || orientation == 'W')
		init_player_ew(cub, orientation);
	cub->player.x = (double)cub->map.player_x + 0.5;
	cub->player.y = (double)cub->map.player_y + 0.5;
}
