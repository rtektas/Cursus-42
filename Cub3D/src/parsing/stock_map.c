/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stock_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 02:18:07 by asaracut          #+#    #+#             */
/*   Updated: 2025/12/07 04:16:51 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cube.h"

static void	verifi_and_scan_line(char *line, t_cub *cub, size_t len, int i)
{
	if ((int)len > cub->map.width_map)
		cub->map.width_map = (int)len;
	cub->map.height_map++;
	while (line[i] && line[i] != '\n')
	{
		if (line[i] == '\t')
			line[i] = ' ';
		if (line[i] != ' ' && line[i] != '1' && line[i] != '0'
			&& line[i] != 'N' && line[i] != 'S'
			&& line[i] != 'E' && line[i] != 'W')
			free_all_exit(cub, "Wrong character in map");
		if (line[i] == 'N' || line[i] == 'S'
			|| line[i] == 'E' || line[i] == 'W')
		{
			if (cub->map.player_dir == 0)
			{
				cub->map.player_dir = line[i];
				cub->map.player_x = i;
				cub->map.player_y = cub->map.height_map - 1;
			}
			cub->map.nombre_player++;
		}
		i++;
	}
}

static int	stock_map_line(char *line, t_cub *cub, size_t len)
{
	char	*dup;

	dup = (char *)malloc(len + 1);
	if (!dup)
		return (-1);
	ft_memcpy(dup, line, len);
	dup[len] = '\0';
	cub->map.map = ft_realloc_matrix(cub->map.map,
			cub->map.height_map - 1, cub->map.height_map);
	if (!cub->map.map)
	{
		free(dup);
		return (-1);
	}
	cub->map.map[cub->map.height_map - 1] = dup;
	return (0);
}

int	stock_map(char *line, t_cub *cub)
{
	size_t	len;

	len = line_len_no_nl(line);
	verifi_and_scan_line(line, cub, len, 0);
	if (stock_map_line(line, cub, len) == -1)
		return (-1);
	return (0);
}
