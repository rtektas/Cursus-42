/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 21:56:05 by asaracut          #+#    #+#             */
/*   Updated: 2025/12/07 04:06:09 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cube.h"

static void	check_zero_neighbors(t_cub *cub, int i, int j)
{
	if (i == 0 || i == cub->map.height_map - 1 || j == 0
		|| j >= (int)ft_strlen(cub->map.map[i]) - 1)
		free_all_exit(cub, "Map is not enclosed by walls");
	if (i - 1 < 0 || i + 1 >= cub->map.height_map)
		free_all_exit(cub, "Map is not enclosed by walls");
	if (j - 1 < 0 || j + 1 >= (int)ft_strlen(cub->map.map[i]))
		free_all_exit(cub, "Map is not enclosed by walls");
	if (j >= (int)ft_strlen(cub->map.map[i - 1])
		|| j >= (int)ft_strlen(cub->map.map[i + 1]))
		free_all_exit(cub, "Map is not enclosed by walls");
	if (cub->map.map[i - 1][j] == ' '
		|| cub->map.map[i + 1][j] == ' '
		|| cub->map.map[i][j - 1] == ' '
		|| cub->map.map[i][j + 1] == ' ')
		free_all_exit(cub, "Map is not enclosed by walls");
}

static void	check_map_line(t_cub *cub, int i)
{
	int	j;

	j = 0;
	while (j < cub->map.width_map)
	{
		if (j >= (int)ft_strlen(cub->map.map[i])
			|| cub->map.map[i][j] == ' ')
		{
			j++;
			continue ;
		}
		if (cub->map.map[i][j] == '0')
			check_zero_neighbors(cub, i, j);
		j++;
	}
}

void	parse_map(t_cub *cub)
{
	int	i;

	if (cub->map.nombre_player != 1)
		free_all_exit(cub, "There must be exactly one player in the map");
	cub->map.map[cub->map.player_y][cub->map.player_x] = '0';
	i = 0;
	while (i < cub->map.height_map)
	{
		check_map_line(cub, i);
		i++;
	}
}
