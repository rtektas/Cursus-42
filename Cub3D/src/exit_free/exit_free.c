/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 22:44:24 by asaracut          #+#    #+#             */
/*   Updated: 2025/12/07 03:49:26 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cube.h"

void	free_textures(t_cub *cub)
{
	if (cub->map.no_texture)
		free(cub->map.no_texture);
	if (cub->map.so_texture)
		free(cub->map.so_texture);
	if (cub->map.we_texture)
		free(cub->map.we_texture);
	if (cub->map.ea_texture)
		free(cub->map.ea_texture);
}

void	free_map(t_cub *cub)
{
	int	i;

	if (!cub->map.map)
		return ;
	i = 0;
	while (i < cub->map.height_map)
	{
		if (cub->map.map[i])
			free(cub->map.map[i]);
		i++;
	}
	free(cub->map.map);
	cub->map.map = NULL;
}

void	free_all(t_cub *cub)
{
	free_textures(cub);
	free_map(cub);
}

void	free_texture_exit(t_cub *cub, char *message)
{
	free_textures(cub);
	exit_error(message, 1);
}

void	free_all_exit(t_cub *cub, char *message)
{
	free_all(cub);
	exit_error(message, 1);
}
